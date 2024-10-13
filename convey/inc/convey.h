#ifndef _CONVEY_H_
#define _CONVEY_H_

#if defined (_WIN32) || defined (WIN32)
# define C_SYSTEM_WIN32
# error "Windows not supported yet"
#elif defined (__unix__) /* defined (_WIN32) || defined (WIN32) */
# define C_SYSTEM_POSIX
# include "c_posix.h"
#else /* defined (__unix__) */
# error "Unsupported operating system"
#endif /* defined (__unix__) */

#include <stdint.h>

typedef __c_ssize_t c_ssize_t;

typedef enum {
	/* individual options */
	C_PERM_USER_READ = 0400,
	C_PERM_USER_WRITE = 0200,
	C_PERM_USER_EXEC = 0100,
	C_PERM_GROUP_READ = 040,
	C_PERM_GROUP_WRITE = 020,
	C_PERM_GROUP_EXEC = 010,
	C_PERM_OTHER_READ = 04,
	C_PERM_OTHER_WRITE = 02,
	C_PERM_OTHER_EXEC = 01,
	/* combined options */
	C_PERM_USER_RWX = 0700,
	C_PERM_GROUP_RWX = 070,
	C_PERM_OTHER_RWX = 07,
} C_Permissions;

typedef enum {
	C_ACCESS_MODE_EXISTS,
	C_ACCESS_MODE_READ,
	C_ACCESS_MODE_WRITE,
	C_ACCESS_MODE_EXEC,
} C_Access_Mode;

typedef enum {
	C_FILE_TYPE_FILE,
	C_FILE_TYPE_DIRECTORY,
	C_FILE_TYPE_SYMLINK,
	C_FILE_TYPE_BLOCK,
	C_FILE_TYPE_CHAR,
	C_FILE_TYPE_FIFO,
	C_FILE_TYPE_SOCKET,
	C_FILE_TYPE_OTHER,
} C_File_Type;

/* TODO improve upon this */
typedef struct {
	C_File_Type type;
} C_Stat;

int c_access (const char *path, C_Access_Mode mode);
int c_isascii (int c);
int c_mkdir (const char *path, C_Permissions perm);
void c_rand_seed (uint32_t seed);
uint32_t c_rand_uint (void);
double c_rand_double (void);
int c_stat (const char *path, C_Stat *out);

#endif /* _CONVEY_H_ */
