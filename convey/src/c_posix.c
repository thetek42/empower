#include "convey.h"
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
        return mkdir (path, (mode_t) perm);
}

void
c_rand_seed (uint32_t seed)
{
	srand48 ((long) seed);
}

uint32_t
c_rand_uint (void)
{
	return (uint32_t) mrand48 ();
}

double
c_rand_double (void)
{
	return drand48 ();
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
	else if (S_ISLNK  (s.st_mode)) out->type = C_FILE_TYPE_SYMLINK;
	else if (S_ISSOCK (s.st_mode)) out->type = C_FILE_TYPE_SOCKET;
	else                           out->type = C_FILE_TYPE_OTHER;

	return ret;
}
