#include "convey.h"
#include <ctype.h>
#include <errhandlingapi.h>
#include <errno.h>
#include <fileapi.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <winerror.h>

static const int access_mode_to_posix_mode[4] = {
	[C_ACCESS_MODE_EXISTS] = F_OK,
	[C_ACCESS_MODE_READ] = R_OK,
	[C_ACCESS_MODE_WRITE] = W_OK,
	[C_ACCESS_MODE_EXEC] = X_OK,
};

int
c_access (const char *path, C_Access_Mode mode)
{
	return access (path, access_mode_to_posix_mode[mode]);
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
	struct stat s;
	int ret;

	if (!out) {
		errno = EINVAL;
		return -1;
	}

	ret = stat (path, &s);
	if (ret < 0) return ret;

	if      (S_ISBLK  (s.st_mode)) out->type = C_FILE_TYPE_BLOCK;
	else if (S_ISCHR  (s.st_mode)) out->type = C_FILE_TYPE_CHAR;
	else if (S_ISDIR  (s.st_mode)) out->type = C_FILE_TYPE_DIRECTORY;
	else if (S_ISFIFO (s.st_mode)) out->type = C_FILE_TYPE_FIFO;
	else if (S_ISREG  (s.st_mode)) out->type = C_FILE_TYPE_FILE;
	/*else if (S_ISLNK  (s.st_mode)) out->type = C_FILE_TYPE_SYMLINK;*/
	else                           out->type = C_FILE_TYPE_OTHER;

	return ret;
}
