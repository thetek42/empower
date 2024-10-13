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

int c_mkdir (const char *path, C_Permissions perm);

#endif /* _CONVEY_H_ */
