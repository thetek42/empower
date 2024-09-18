#ifndef _EMPOWER_H_
#define _EMPOWER_H_

#include "config.h"

/* c version checks ***********************************************************/

#define E_STDC_VERSION_C23 202000L /* to be replaced with 202311L in the future */
#define E_STDC_VERSION_C11 201112L
#define E_STDC_VERSION_C99 199901L
#define E_STDC_VERSION_C89 198900L

#ifdef __STDC_VERSION__
# define E_STDC_VERSION __STDC_VERSION__
#else /* __STDC_VERSION__ */
# define E_STDC_VERSION E_STDC_VERSION_C89
#endif /* __STDC_VERSION__ */

/* config checks **************************************************************/

#if E_CONFIG_TEST_TYPE_MACROS && (E_STDC_VERSION < E_STDC_VERSION_C23)
# error "E_CONFIG_TEST_TYPE_MACROS requires C23 or later"
#endif /* E_CONFIG_TEST_TYPE_MACROS && (E_STDC_VERSION < E_STDC_VERSION_C23) */

/* includes *******************************************************************/

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if E_STDC_VERSION >= E_STDC_VERSION_C99
# include <inttypes.h>
# if E_STDC_VERSION < E_STDC_VERSION_C23
#  include <stdbool.h>
# endif /* E_STDC_VERSION < E_STDC_VERSION_C23 */
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

#if E_CONFIG_HAS_SYS_TYPES
# include <sys/types.h>
#endif

/* c feature backports ********************************************************/

#if E_STDC_VERSION < E_STDC_VERSION_C23
# define nullptr NULL
typedef void *nullptr_t;
#endif /* E_STDC_VERSION < E_STDC_VERSION_C23 */

#if E_STDC_VERSION < E_STDC_VERSION_C11
# define static_assert(expr, msg) typedef char __e_static_assertion[(expr) ? 1 : -1]
#endif /* E_STDC_VERSION < E_STDC_VERSION_C11 */

#if !E_CONFIG_HAS_SYS_TYPES
typedef ptrdiff_t ssize_t;
static_assert (sizeof (isize) == sizeof (usize));
#endif /* !E_CONFIG_HAS_SYS_TYPES */

#if E_STDC_VERSION < E_STDC_VERSION_C99
# define restrict
#endif /* E_STDC_VERSION < E_STDC_VERSION_C99 */

#if E_STDC_VERSION < E_STDC_VERSION_C99
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef ssize_t intptr_t;
typedef size_t uintptr_t;
typedef uint8_t bool;
# define false 0
# define true 1
# define PRId8 "d"
# define PRIi8 "i"
# define PRIu8 "u"
# define PRIo8 "o"
# define PRIx8 "x"
# define PRIX8 "X"
# define PRId16 "d"
# define PRIi16 "i"
# define PRIu16 "u"
# define PRIo16 "o"
# define PRIx16 "x"
# define PRIX16 "X"
# define PRId32 "d"
# define PRIi32 "i"
# define PRIu32 "u"
# define PRIo32 "o"
# define PRIx32 "x"
# define PRIX32 "X"
# define PRId64 "ld"
# define PRIi64 "li"
# define PRIu64 "lu"
# define PRIo64 "lo"
# define PRIx64 "lx"
# define PRIX64 "lX"
# define PRIdPTR "ld"
# define PRIiPTR "li"
# define PRIuPTR "lu"
# define PRIoPTR "lo"
# define PRIxPTR "lx"
# define PRIXPTR "lX"
#endif /* E_STDC_VERSION < E_STDC_VERSION_C99 */

#if E_STDC_VERSION >= E_STDC_VERSION_C99
# define E_FMT_USIZE "zu"
# define E_FMT_ISIZE "zi"
#else /* E_STDC_VERSION >= E_STDC_VERSION_C99 */
# define E_FMT_USIZE "lu"
# define E_FMT_ISIZE "li"
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

/* macro utils ****************************************************************/

#if E_CONFIG_MODULE_MACRO
# define E_MACRO_STRINGIFY(x) E_MACRO_STRINGIFY_HELPER (x)
# define E_MACRO_STRINGIFY_HELPER(x) #x
#endif /* E_CONFIG_MODULE_MACRO */

/* common types ***************************************************************/

typedef int8_t i8;
typedef int16_t i16;
typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
#if E_CONFIG_HAS_INT_128
typedef signed __int128 i128;
typedef unsigned __int128 u128;
#endif /* E_CONFIG_HAS_INT_128 */

typedef signed char ichar;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef size_t usize;
typedef ssize_t isize;

#if E_STDC_VERSION >= E_STDC_VERSION_C99
typedef signed long long llong;
typedef unsigned long long ullong;
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

#if E_CONFIG_HAS_FLOAT
typedef float f32;
#endif /* E_CONFIG_HAS_FLOAT */
#if E_CONFIG_HAS_DOUBLE
typedef double f64;
#endif /* E_CONFIG_HAS_DOUBLE */
#if E_CONFIG_HAS_LONG_DOUBLE
typedef long double f128;
#endif /* E_CONFIG_HAS_LONG_DOUBLE */

/* results ********************************************************************/

#if E_CONFIG_MODULE_RESULT

typedef isize e_result_t;

enum {
	E_OK                            = 0,
	E_ERR_NOT_PERMITTED             = -EPERM,
	E_ERR_NO_SUCH_FILE              = -ENOENT,
	E_ERR_NO_SUCH_PROCESS           = -ESRCH,
	E_ERR_INTERRUPTED               = -EINTR,
	E_ERR_IO_ERROR                  = -EIO,
	E_ERR_NO_SUCH_ADDRESS           = -ENXIO,
	E_ERR_TOO_BIG                   = -E2BIG,
	E_ERR_FORMAT_ERROR              = -ENOEXEC,
	E_ERR_BAD_FILE                  = -EBADF,
	E_ERR_NO_CHILD_PROCESS          = -ECHILD,
	E_ERR_TRY_AGAIN                 = -EAGAIN,
	E_ERR_OUT_OF_MEMORY             = -ENOMEM,
	E_ERR_PERMISSION_DENIED         = -EACCES,
	E_ERR_BAD_ADDRESS               = -EFAULT,
	E_ERR_NOT_A_BLOCK_DEVICE        = -ENOTBLK,
	E_ERR_BUSY                      = -EBUSY,
	E_ERR_EXISTS                    = -EEXIST,
	E_ERR_CROSS_DEVICE_LINK         = -EXDEV,
	E_ERR_NO_SUCH_DEVICE            = -ENODEV,
	E_ERR_NOT_A_DIRECTORY           = -ENOTDIR,
	E_ERR_IS_A_DIRECTORY            = -EISDIR,
	E_ERR_INVALID_ARGUMENT          = -EINVAL,
	E_ERR_TABLE_OVERFLOW            = -ENFILE,
	E_ERR_TOO_MANY_FILES            = -EMFILE,
	E_ERR_NOT_A_TTY                 = -ENOTTY,
	E_ERR_TEXT_FILE_BUSY            = -ETXTBSY,
	E_ERR_FILE_TOO_LARGE            = -EFBIG,
	E_ERR_NO_SPACE_LEFT             = -ENOSPC,
	E_ERR_ILLEGAL_SEEK              = -ESPIPE,
	E_ERR_READ_ONLY                 = -EROFS,
	E_ERR_TOO_MANY_LINKS            = -EMLINK,
	E_ERR_BROKEN_PIPE               = -EPIPE,
	E_ERR_MATH_DOMAIN               = -EDOM,
	E_ERR_MATH_RANGE                = -ERANGE,
	E_ERR_DEADLOCK                  = -EDEADLK,
	E_ERR_FILE_NAME_TOO_LONG        = -ENAMETOOLONG,
	E_ERR_NO_LOCKS_AVAILABLE        = -ENOLCK,
	E_ERR_INVALID_SYSCALL           = -ENOSYS,
	E_ERR_NOT_EMPTY                 = -ENOTEMPTY,
	E_ERR_SYMLINK_LOOP              = -ELOOP,
	E_ERR_WOULD_BLOCK               = -EWOULDBLOCK,
	E_ERR_NO_MESSAGE                = -ENOMSG,
	E_ERR_ID_REMOVED                = -EIDRM,
	E_ERR_OUT_OF_RANGE              = -ECHRNG,
	E_ERR_L2_NOT_SYNCHRONIZED       = -EL2NSYNC,
	E_ERR_L3_HALTED                 = -EL3HLT,
	E_ERR_L3_RESET                  = -EL3RST,
	E_ERR_LINK_OUT_OF_RANGE         = -ELNRNG,
	E_ERR_DRIVER_NOT_ATTACHED       = -EUNATCH,
	E_ERR_NO_CSI_AVAILABLE          = -ENOCSI,
	E_ERR_L2_HALTED                 = -EL2HLT,
	E_ERR_BAD_EXCHANGE              = -EBADE,
	E_ERR_BAD_REQUEST               = -EBADR,
	E_ERR_EXCHANGE_FULL             = -EXFULL,
	E_ERR_NO_ANODE                  = -ENOANO,
	E_ERR_INVALID_REQUEST_CODE      = -EBADRQC,
	E_ERR_INVALID_SLOT              = -EBADSLT,
	E_ERR_DEADLOCK_2                = -EDEADLOCK,
	E_ERR_INVALID_FONT_FILE         = -EBFONT,
	E_ERR_NOT_A_STREAM              = -ENOSTR,
	E_ERR_NOT_AVAILABLE             = -ENODATA,
	E_ERR_EXPIRED                   = -ETIME,
	E_ERR_OUT_OF_STREAM_RESOURCES   = -ENOSR,
	E_ERR_NOT_ON_NETWORK            = -ENONET,
	E_ERR_NOT_INSTALLED             = -ENOPKG,
	E_ERR_IS_REMOTE                 = -EREMOTE,
	E_ERR_SEVERED_LINK              = -ENOLINK,
	E_ERR_ADVERTISE                 = -EADV,
	E_ERR_SRMOUNT                   = -ESRMNT,
	E_ERR_COMMUNICATION             = -ECOMM,
	E_ERR_PROTOCOL                  = -EPROTO,
	E_ERR_MULTIHOP                  = -EMULTIHOP,
	E_ERR_RFS_SPECIFIC              = -EDOTDOT,
	E_ERR_INVALID_MESSAGE           = -EBADMSG,
	E_ERR_OVERFLOW                  = -EOVERFLOW,
	E_ERR_NOT_UNIQUE                = -ENOTUNIQ,
	E_ERR_BAD_FILE_DESCRIPTOR       = -EBADFD,
	E_ERR_REMOTE_CHANGED            = -EREMCHG,
	E_ERR_LIB_ACCESS                = -ELIBACC,
	E_ERR_LIB_CORRUPTED             = -ELIBBAD,
	E_ERR_LIB_SECTION_CORRUPTED     = -ELIBSCN,
	E_ERR_TOO_MANY_LIBS             = -ELIBMAX,
	E_ERR_CANNOT_EXEC_LIB           = -ELIBEXEC,
	E_ERR_ILLEGAL_BYTES             = -EILSEQ,
	E_ERR_RESTART_SYSCALL           = -ERESTART,
	E_ERR_STREAM_PIPE               = -ESTRPIPE,
	E_ERR_TOO_MANY_USERS            = -EUSERS,
	E_ERR_NOT_A_SOCKET              = -ENOTSOCK,
	E_ERR_NO_DESTINATION            = -EDESTADDRREQ,
	E_ERR_MESSAGE_TOO_LONG          = -EMSGSIZE,
	E_ERR_INVALID_PROTOCOL_TYPE     = -EPROTOTYPE,
	E_ERR_PROTOCOL_NOT_AVAILABLE    = -ENOPROTOOPT,
	E_ERR_PROTOCOL_NOT_SUPPORTED    = -EPROTONOSUPPORT,
	E_ERR_SOCKET_TYPE_NOT_SUPPORTED = -ESOCKTNOSUPPORT,
	E_ERR_OPERATION_NOT_SUPPORTED   = -EOPNOTSUPP,
	E_ERR_BAD_PROTOCOL_FAMILY       = -EPFNOSUPPORT,
	E_ERR_BAD_ADDRESS_FAMILY        = -EAFNOSUPPORT,
	E_ERR_ADDRESS_IN_USE            = -EADDRINUSE,
	E_ERR_ADDRESS_NOT_AVAILABLE     = -EADDRNOTAVAIL,
	E_ERR_NETWORK_DOWN              = -ENETDOWN,
	E_ERR_NETWORK_UNREACH           = -ENETUNREACH,
	E_ERR_NETWORK_RESET             = -ENETRESET,
	E_ERR_CONNECTION_ABORTED        = -ECONNABORTED,
	E_ERR_CONNECTION_RESET          = -ECONNRESET,
	E_ERR_NO_BUFFER_SPACE           = -ENOBUFS,
	E_ERR_ALREADY_CONNECTED         = -EISCONN,
	E_ERR_NOT_CONNECTED             = -ENOTCONN,
	E_ERR_IS_SHUTDOWN               = -ESHUTDOWN,
	E_ERR_TOO_MANY_REFERENCES       = -ETOOMANYREFS,
	E_ERR_TIMEOUT                   = -ETIMEDOUT,
	E_ERR_CONNECTION_REFUSED        = -ECONNREFUSED,
	E_ERR_HOST_DOWN                 = -EHOSTDOWN,
	E_ERR_HOST_UNREACHABLE          = -EHOSTUNREACH,
	E_ERR_ALREADY_IN_PROGRESS       = -EALREADY,
	E_ERR_IN_PROGRESS               = -EINPROGRESS,
	E_ERR_STALE_FILE_HANDLE         = -ESTALE,
	E_ERR_NEEDS_CLEANING            = -EUCLEAN,
	E_ERR_NOT_A_NAMED_TYPE_FILE     = -ENOTNAM,
	E_ERR_NO_SEMAPHORES_AVAILABLE   = -ENAVAIL,
	E_ERR_IS_A_NAMED_TYPE_FILE      = -EISNAM,
	E_ERR_REMOTE_IO                 = -EREMOTEIO,
	E_ERR_QUOTA_EXCEEDED            = -EDQUOT,
	E_ERR_NO_MEDIUM                 = -ENOMEDIUM,
	E_ERR_WRONG_MEDIUM_TYPE         = -EMEDIUMTYPE,
	E_ERR_CANCELED                  = -ECANCELED,
	E_ERR_KEY_NOT_AVAILABLE         = -ENOKEY,
	E_ERR_KEY_EXPIRED               = -EKEYEXPIRED,
	E_ERR_KEY_REVOKED               = -EKEYREVOKED,
	E_ERR_KEY_REJECTED              = -EKEYREJECTED,
	E_ERR_OWNER_DIED                = -EOWNERDEAD,
	E_ERR_NOT_RECOVERABLE           = -ENOTRECOVERABLE,
	E_ERR_RFKILL                    = -ERFKILL,
	E_ERR_HARDWARE_ERROR            = -EHWPOISON
};

#endif /* E_CONFIG_MODULE_RESULT */

/* logging ********************************************************************/

#if E_CONFIG_MODULE_LOG

# if E_STDC_VERSION >= E_STDC_VERSION_C23
#  define e_log_debug(fmt, ...) fprintf (stderr, "\x1b[35mDEBUG \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
#  define e_log_info(fmt, ...)  fprintf (stderr, "\x1b[32mINFO  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
#  define e_log_warn(fmt, ...)  fprintf (stderr, "\x1b[33mWARN  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
#  define e_log_error(fmt, ...) fprintf (stderr, "\x1b[31mERROR \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
#  define e_die(fmt, ...)                                                       \
	do {                                                                   \
		e_log_error (fmt __VA_OPT__ (,) __VA_ARGS__);                  \
		exit (EXIT_FAILURE);                                           \
	} while (0)
# else /* E_STDC_VERSION >= E_STDC_VERSION_C23 */
enum __e_log_level {
	__E_LOG_LEVEL_DEBUG,
	__E_LOG_LEVEL_INFO,
	__E_LOG_LEVEL_WARN,
	__E_LOG_LEVEL_ERROR
};
#  if E_STDC_VERSION >= E_STDC_VERSION_C99
#   define e_log_debug(...) __e_log_impl (__E_LOG_LEVEL_DEBUG, " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
#   define e_log_info(...)  __e_log_impl (__E_LOG_LEVEL_INFO,  " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
#   define e_log_warn(...)  __e_log_impl (__E_LOG_LEVEL_WARN,  " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
#   define e_log_error(...) __e_log_impl (__E_LOG_LEVEL_ERROR, " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
#   define e_die(...)                                                            \
	do {                                                                   \
		e_log_error (__VA_ARGS__);                                     \
		exit (EXIT_FAILURE);                                           \
	} while (0)
void __e_log_impl (enum __e_log_level level, const char *formatted_file_pos, const char *fmt, ...);
#  else /* E_STDC_VERSION >= E_STDC_VERSION_C99 */
void e_log_debug (const char *fmt, ...);
void e_log_info (const char *fmt, ...);
void e_log_warn (const char *fmt, ...);
void e_log_error (const char *fmt, ...);
void e_die (const char *fmt, ...);
#  endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */
# endif /* E_STDC_VERSION >= E_STDC_VERSION_C23 */

#endif /* E_CONFIG_MODULE_LOG */

/* cstr ***********************************************************************/

#if E_CONFIG_MODULE_CSTR

typedef int (* e_char_predicate_t) (int c);

usize e_cstr_count_char (const char *s, char c);
usize e_cstr_count_func (const char *s, e_char_predicate_t func);
bool e_cstr_is_ascii (const char *s);
bool e_cstr_is_blank (const char *s);
bool e_cstr_matches_predicate (const char *s, e_char_predicate_t func);
char *e_cstr_to_ascii_lower (char *s);
char *e_cstr_to_ascii_upper (char *s);
char *e_cstr_to_ascii_lower_buf (const char *restrict src, char *restrict dest);
char *e_cstr_to_ascii_upper_buf (const char *restrict src, char *restrict dest);

int e_char_is_ascii (int c);

#endif /* E_CONFIG_MODULE_CSTR */

/* debugging utilities ********************************************************/

#if E_CONFIG_MODULE_DEBUG

# if E_STDC_VERSION >= E_STDC_VERSION_C11
#  if !E_CONFIG_MODULE_MACRO
#   error "module e_debug depends on: e_macro"
#  endif /* !E_CONFIG_MODULE_MACRO */

#  define E_DEBUG_AUTO_FMT(value) _Generic ((value),                           \
		i8: "%" PRIi8,                                                 \
		i16: "%" PRIi16,                                               \
		i32: "%" PRIi32,                                               \
		i64: "%" PRIi64,                                               \
		u8: "%" PRIu8,                                                 \
		u16: "%" PRIu16,                                               \
		u32: "%" PRIu32,                                               \
		u64: "%" PRIu64,                                               \
		f32: "%f",                                                     \
		f64: "%f",                                                     \
		bool: "%d",                                                    \
		default: "%p")

#  define e_debug(value)                                                        \
	do {                                                                   \
		fprintf (stderr, "\x1b[35mDEBUG \x1b[0m"                       \
		         E_MACRO_STRINGIFY (value) " = \x1b[35m");             \
		fprintf (stderr, E_DEBUG_AUTO_FMT(value), (value));            \
		fprintf (stderr, "\x1b[0m \x1b[90m(" __FILE__ ":"              \
		         E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n");           \
	} while (0)
# endif /* E_STDC_VERSION >= E_STDC_VERSION_C11 */

#endif /* E_CONFIG_MODULE_DEBUG */

/* alloc **********************************************************************/

#if E_CONFIG_MODULE_ALLOC

# define e_alloc(type, nmemb) __e_mem_alloc (sizeof (type) * (nmemb))
# define e_alloc_size(size) __e_mem_alloc ((size))
# define e_alloc_zero(type, nmemb) __e_mem_calloc ((nmemb), sizeof (type))
# define e_alloc_zero_size(size, nmemb) __e_mem_calloc ((nmemb), (size))
# define e_realloc(ptr, type, nmemb) __e_mem_realloc ((ptr), sizeof (type) * (nmemb))
# define e_realloc_size(ptr, size) __e_mem_realloc ((ptr), (size))
# define e_free(ptr) free ((ptr))
# define e_new(type) e_alloc (type, 1)
# define e_new_zero(type) e_alloc_zero (type, 1)

void *__e_mem_alloc (usize size);
void *__e_mem_calloc (usize nmemb, usize size);
void *__e_mem_realloc (void *ptr, usize size);

#endif /* E_CONFIG_MODULE_ALLOC */

/* testing ********************************************************************/

#if E_CONFIG_MODULE_TEST

struct __e_test_result {
	usize success;
	usize failure;
};

extern struct __e_test_result __e_global_test;

# define E_TEST_MAIN() struct __e_test_result __e_global_test = {0}

# define __E_TEST_ASSERT_FMT(name) "\x1b[31mfail\x1b[0m \x1b[33m" name "\x1b[0m (" __FILE__ ":%d) "
# define __E_TEST_SPACE "                                                                      "
# define __E_TEST_PRINT_ASSERT_FN(name) fprintf (stderr, "\x1b[35m%s\x1b[0m ", name)

# define __e_test_assert(type, name, expr)                                     \
	do {                                                                   \
		type success = (expr);                                         \
		if (success) {                                                 \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name),   \
			                 __LINE__);                            \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l, __E_TEST_SPACE);           \
			__E_TEST_PRINT_ASSERT_FN ("assert");                   \
			fprintf (stderr, #expr "\n");                          \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)

# define __e_test_assert_eq(type, type_cast, name, expr, check)                \
	do {                                                                   \
		type result = (expr);                                          \
		type other = (type_cast) (check);                              \
		if (result == other) {                                         \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name),   \
			                 __LINE__);                            \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_eq");                \
			fprintf (stderr, #expr " \x1b[36mgot\x1b[0m ");        \
			fprintf (stderr, E_DEBUG_AUTO_FMT (result), result);   \
			fprintf (stderr, " \x1b[36mwant\x1b[0m ");             \
			fprintf (stderr, E_DEBUG_AUTO_FMT (other), other);     \
			fprintf (stderr, "\n");                                \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)

# define __e_test_assert_str_eq(name, expr, check)                             \
	do {                                                                   \
		const char *result = (expr);                                   \
		const char *other = (check);                                   \
		if (strcmp (result, other) == 0) {                             \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name),   \
			                 __LINE__);                            \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_str_eq");            \
			fprintf (stderr, #expr " \x1b[36mgot\x1b[0m \"%s\" "   \
			         "\x1b[36mwant\x1b[0m \"%s\"\n", result,       \
			         other);                                       \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)                                                                                

# define e_test_finish()                                                       \
	do {                                                                   \
		f64 total_perc = (f64) (__e_global_test.success +              \
		                        __e_global_test.failure) / 100.0;      \
		f64 succ_rate = (f64) __e_global_test.success / total_perc;    \
		f64 fail_rate = (f64) __e_global_test.failure / total_perc;    \
		fprintf (stderr, "\n=======================================\n" \
		         "TEST SUMMARY:"                                       \
		         "\n - total:   %"E_FMT_USIZE                          \
			 "\n - \x1b[32msuccess: %"E_FMT_USIZE" (%.2f%%)\x1b[0m"\
			 "\n - \x1b[31mfailure: %"E_FMT_USIZE" (%.2f%%)\x1b[0m"\
		         "\n=======================================\n",        \
			 __e_global_test.success + __e_global_test.failure,    \
			 __e_global_test.success, succ_rate,                   \
			 __e_global_test.failure, fail_rate);                  \
		exit (__e_global_test.failure > 0 ? EXIT_FAILURE :             \
		      EXIT_SUCCESS);                                           \
	} while (0)

# if (E_STDC_VERSION >= E_STDC_VERSION_C23) && !E_CONFIG_TEST_TYPE_MACROS && !defined (E_CONFIG_TEST_TYPE_MACROS_OVERRIDE)
#  define e_test_assert(name, expr) __e_test_assert (auto, name, expr)
#  define e_test_assert_eq(name, expr, check) __e_test_assert_eq (auto, typeof (result), name, expr, check)
#  define e_test_assert_str_eq(name, expr, check) __e_test_assert_str_eq (name, expr, check)
# elif E_STDC_VERSION >= E_STDC_VERSION_C11 /* (E_STDC_VERSION >= E_STDC_VERSION_C23) && !E_CONFIG_TEST_TYPE_MACROS */
#  define e_test_assert(name, type, expr) __e_test_assert (type, name, expr)
#  define e_test_assert_eq(name, type, expr, check) __e_test_assert_eq (type, type, name, expr, check)
#  define e_test_assert_str_eq(name, expr, check) __e_test_assert_str_eq (name, expr, check)
# else /* E_STDC_VERSION >= E_STDC_VERSION_C11 */
#  define e_test_assert(name, type, expr) __e_test_assert (type, name, expr)
#  define e_test_assert_eq(name, type, expr, check) __e_test_assert (type, name, (expr) == (check))
#  define e_test_assert_str_eq(name, expr, check) __e_test_assert_str_eq (name, expr, check)
# endif /* E_STDC_VERSION >= E_STDC_VERSION_C11 */

#endif /* E_CONFIG_MODULE_TEST */

#endif /* _EMPOWER_H_ */
