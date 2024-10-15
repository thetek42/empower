#include "convey.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ctype.h>
#include <errhandlingapi.h>
#include <errno.h>
#include <fileapi.h>
#include <io.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <winerror.h>

static const int access_mode_to_posix_mode[4] = {
	[C_ACCESS_MODE_EXISTS] = 00,
	[C_ACCESS_MODE_READ] = 04,
	[C_ACCESS_MODE_WRITE] = 02,
	[C_ACCESS_MODE_EXEC] = 04,
};

int
c_access (const char *path, C_Access_Mode mode)
{
	return _access (path, access_mode_to_posix_mode[mode]);
}

int
c_isascii (int c)
{
	return isascii (c);
}

int
c_mkdir (const char *path, C_Permissions perm)
{
	DWORD error;
	BOOL win_ret;

	(void) perm; /* TODO */

	win_ret = CreateDirectoryA (path, nullptr);
	if (win_ret == 0) {
		error = GetLastError ();
		switch (error) {
		case ERROR_ALREADY_EXISTS: return errno = EEXIST, -1;
		case ERROR_PATH_NOT_FOUND: return errno = ENOENT, -1;
		default:                   return errno = EACCES, -1; /* idk */
		}
	}

	return 0;
}

void
c_rand_seed (uint32_t seed)
{
	srand ((unsigned int) seed);
}

uint32_t
c_rand_uint (void)
{
#if RAND_MAX != 0x7FFF
# error "RAND_MAX is expected to be 0x7FFF on MINGW."
#endif /* RAND_MAX != 0x7FFF */

	return ((uint32_t) rand () << 30) | ((uint32_t) rand () << 15) |
	       ((uint32_t) rand ());
}

double
c_rand_double (void)
{
	return ((double) rand ()) * (1.0 / RAND_MAX);
}

int
c_stat (const char *path, C_Stat *out)
{
	struct __stat64 s;
	int ret;

	if (!out) {
		errno = EINVAL;
		return -1;
	}

	ret = _stat64 (path, &s);
	if (ret < 0) return ret;

	if      (s.st_mode & _S_IFREG) out->type = C_FILE_TYPE_FILE;
	else if (s.st_mode & _S_IFDIR) out->type = C_FILE_TYPE_DIRECTORY;
	else                           out->type = C_FILE_TYPE_OTHER;

	return ret;
}
