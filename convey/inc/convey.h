#ifndef _CONVEY_H_
#define _CONVEY_H_

/* c version checks ***********************************************************/

#define C_STDC_VERSION_C99 199901L
#define C_STDC_VERSION_C11 201112L
#define C_STDC_VERSION_C17 201710L
#define C_STDC_VERSION_C23 202000L /* should be 202311L in the future */

#if !defined (__STDC_VERSION__) || __STDC_VERSION__ < C_STDC_VERSION_C99
# error "A minimum of C99 is required."
#endif /* !defined (__STDC_VERSION__) || __STDC_VERSION__ < C_STDC_VERSION_C99 */

#define C_STDC_VERSION __STDC_VERSION__

/* compiler checks ************************************************************/

#if defined (__clang__)
# define C_COMPILER_CLANG
#elif defined (__GNUC__) /* defined (__clang__) */
# define C_COMPILER_GCC
#elif defined (_MSC_VER) /* defined (__GNUC__) */
# define C_COMPILER_MSVC
#else /* defined (_MSC_VER) */
# error "Unsupported compiler"
#endif /*defined (_MSC_VER) */

/* operating system checks ****************************************************/

#if defined (__MINGW32__)
# define C_SYSTEM_MINGW
# include "c_mingw.h"
#elif defined (_WIN32) || defined (WIN32) /* defined (__MINGW32__) */
# define C_SYSTEM_WIN32
# include "c_win32.h"
#elif defined (__unix__) /* defined (_WIN32) || defined (WIN32) */
# define C_SYSTEM_POSIX
# include "c_posix.h"
#else /* defined (__unix__) */
# error "Unsupported operating system"
#endif /* defined (__unix__) */

/* includes *******************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

/* utility macros *************************************************************/

#define __C_CONCAT_HELPER(a, b) a##b
#define __C_CONCAT(a, b) __C_CONCAT_HELPER (a, b)

/* c23 backports **************************************************************/

#if C_STDC_VERSION < C_STDC_VERSION_C23 || defined (C_COMPILER_MSVC)
# include <stdbool.h>
# define nullptr ((nullptr_t) NULL)
typedef void *nullptr_t; /* not exactly the same behaviour but whatever */
#endif /* C_STDC_VERSION < C_STDC_VERSION_C23 || defined (C_COMPILER_MSVC) */

#if C_STDC_VERSION >= C_STDC_VERSION_C11 && C_STDC_VERSOIN < C_STDC_VERSION_C23
# include <stdalign.h>
#endif /* C_STDC_VERSION >= C_STDC_VERSION_C11 && C_STDC_VERSOIN < C_STDC_VERSION_C23 */

#if C_STDC_VERSION >= C_STDC_VERSION_C23 && !defined (C_SYSTEM_MINGW) && !defined (C_SYSTEM_WIN32)
# define __C_STDBIT_SUPPORTED 1
# include <stdbit.h>
#else /* C_STDC_VERSION < C_STDC_VERSION_C23 && !defined (C_SYSTEM_MINGW) && !defined (C_SYSTEM_WIN32) */
# define __C_STDBIT_SUPPORTED 0
/* TODO better stdbit.h */
uint64_t stdc_bit_ceil_u64 (uint64_t x);
# if C_STDC_VERSION >= C_STDC_VERSION_C11
#  define stdc_bit_ceil(x) _Generic ((x),                                      \
	uint8_t:  (uint8_t)  stdc_bit_ceil_u64 (x),                            \
	uint16_t: (uint16_t) stdc_bit_ceil_u64 (x),                            \
	uint32_t: (uint32_t) stdc_bit_ceil_u64 (x),                            \
	uint64_t: (uint64_t) stdc_bit_ceil_u64 (x))
# endif /* C_STDC_VERSION >= C_STDC_VERSION_C11 */
#endif /* C_STDC_VERSION < C_STDC_VERSION_C23 && !defined (C_SYSTEM_MINGW) */

/* c11 backports **************************************************************/

#if C_STDC_VERSION < C_STDC_VERSION_C11
# if defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG)
#  define static_assert(expr, msg) typedef char __C_CONCAT (__c_static_assert_, __COUNTER__)[(expr) ? 1 : -1]
# else /* defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG) */
#  define static_assert(expr, msg) typedef char __C_CONCAT (__c_static_assert_, __LINE__)[(expr) ? 1 : -1]
# endif /* defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG) */
# if defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG)
#  define alignof(expr) __alignof__ (expr)
# elif defined (C_COMPILER_MSVC) /* defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG) */
#  define alignof(expr) __alignof (expr)
# else /* defined (C_COMPILER_MSVC) */
#  define __E_ALIGNOF_NOT_SUPPORTED
# endif /* defined (C_COMPILER_MSVC) */
#endif /* C_STDC_VERSION < C_STDC_VERSION_C11 */

/* attributes *****************************************************************/

#if C_STDC_VERSION >= C_STDC_VERSION_C23
# define C_ATTR_DEPRECATED(...) [[deprecated (__VA_ARGS__)]]
# define C_ATTR_FALLTHROUGH [[fallthrough]]
# define C_ATTR_MAYBE_UNUSED [[maybe_unused]]
# define C_ATTR_NODISCARD(...) [[nodiscard (__VA_ARGS__)]]
# define C_ATTR_NORETURN [[noreturn]]
# define C_ATTR_REPRODUCIBLE [[reproducible]]
# define C_ATTR_UNSEQUENCED [[unsequenced]]
# if defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG)
#  define C_ATTR_FORMAT(function, fmt_arg, va_arg) [[gnu::format (function, fmt_arg, va_arg)]]
# else /* defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG) */
#  define C_ATTR_FORMAT(function, fmt_arg, va_arg)
# endif /* defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG) */
#else /* C_STDC_VERSION < C_STDC_VERSION_C23 */
# define C_ATTR_NORETURN _Noreturn
# if defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG)
#  define C_ATTR_DEPRECATED(...) __attribute__ ((deprecated (__VA_ARGS__)))
#  define C_ATTR_FALLTHROUGH __attribute__ ((fallthrough))
#  define C_ATTR_MAYBE_UNUSED __attribute__ ((unused))
#  define C_ATTR_NODISCARD(...) __attribute__ ((warn_unused_result))
#  define C_ATTR_REPRODUCIBLE __attribute__ ((reproducible))
#  define C_ATTR_UNSEQUENCED __attribute__ ((unsequenced))
#  define C_ATTR_FORMAT(function, fmt_arg, va_arg) __attribute__ ((format (function, fmt_arg, va_arg)))
# else /* defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG) */
#  define C_ATTR_DEPRECATED(...) __declspec (deprecated (__VA_ARGS__))
#  define C_ATTR_FALLTHROUGH
#  define C_ATTR_MAYBE_UNUSED
#  define C_ATTR_NODISCARD(...)
#  define C_ATTR_REPRODUCIBLE
#  define C_ATTR_UNSEQUENCED
#  define C_ATTR_FORMAT(function, fmt_arg, va_arg)
# endif /* defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG) */
#endif /* C_STDC_VERSION < C_STDC_VERSION_C23 */

/* miscellaneous **************************************************************/

#if C_STDC_VERSION >= C_STDC_VERSION_C11
# define C_MAX_ALIGN alignof (max_align_t)
#elif defined (C_COMPILER_GCC) || defined (C_COMPILER_CLANG) /* C_STDC_VERSION >= C_STDC_VERSION_C11 */
# define C_MAX_ALIGN __BIGGEST_ALIGNMENT__
#else /* defined (C_COMPILER_GCC) || define (C_COMPILER_CLANG) */
# define C_MAX_ALIGN sizeof (uint64_t)
#endif /* defined (C_COMPILER_GCC) || define (C_COMPILER_CLANG) */

/* msvc stupidity *************************************************************/

#if defined (C_COMPILER_MSVC)
# define strdup _strdup
#endif /* defined (C_COMPILER_MSVC) */

/* types **********************************************************************/

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

/* TODO this should probably be flag enum i guess */
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

/* functions ******************************************************************/

int c_access (const char *path, C_Access_Mode mode);
int c_isascii (int c);
int c_mkdir (const char *path, C_Permissions perm);
void c_rand_seed (uint32_t seed);
uint32_t c_rand_uint (void);
double c_rand_double (void);
int c_stat (const char *path, C_Stat *out);

#endif /* _CONVEY_H_ */
