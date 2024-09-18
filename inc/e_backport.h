#ifndef _EMPOWER_BACKPORT_H_
#define _EMPOWER_BACKPORT_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

/* c23 features ***************************************************************/

#if E_STDC_VERSION < E_STDC_VERSION_C23

#define nullptr NULL

typedef void *nullptr_t;

#endif /* E_STDC_VERSION < E_STDC_VERSION_C23 */

/* c11 features ***************************************************************/

#if E_STDC_VERSION < E_STDC_VERSION_C11

#define static_assert(expr, msg) typedef char __e_static_assertion[(expr) ? 1 : -1]

#endif /* E_STDC_VERSION < E_STDC_VERSION_C11 */

/* c99 features ***************************************************************/

#if E_STDC_VERSION < E_STDC_VERSION_C99

#if E_CONFIG_C89_HAS_RESTRICT
# define restrict __restrict
#else /* E_CONFIG_C89_HAS_RESTRICT */
# define restrict
#endif /* E_CONFIG_C89_HAS_RESTRICT */

typedef signed char int8_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef signed long intmax_t;
typedef unsigned long uintmax_t;
typedef ssize_t intptr_t;
typedef size_t uintptr_t;

#define PRId8 "d"
#define PRIi8 "i"
#define PRIu8 "u"
#define PRIo8 "o"
#define PRIx8 "x"
#define PRIX8 "X"
#define PRId16 "d"
#define PRIi16 "i"
#define PRIu16 "u"
#define PRIo16 "o"
#define PRIx16 "x"
#define PRIX16 "X"
#define PRIdPTR "ld"
#define PRIiPTR "li"
#define PRIuPTR "lu"
#define PRIoPTR "lo"
#define PRIxPTR "lx"
#define PRIXPTR "lX"

#if E_CONFIG_C89_INT_IS_32_BIT
typedef signed int int32_t;
typedef unsigned int uint32_t;
# define PRId32 "d"
# define PRIi32 "i"
# define PRIu32 "u"
# define PRIo32 "o"
# define PRIx32 "x"
# define PRIX32 "X"
#else /* E_CONFIG_C89_INT_IS_32_BIT */
typedef signed long int32_t;
typedef unsigned long uint32_t;
# define PRId32 "ld"
# define PRIi32 "li"
# define PRIu32 "lu"
# define PRIo32 "lo"
# define PRIx32 "lx"
# define PRIX32 "lX"
#endif /* E_CONFIG_C89_INT_IS_32_BIT */

#if E_CONFIG_C89_LONG_IS_64_BIT
typedef signed long int64_t;
typedef unsigned long uint64_t;
# define PRId64 "ld"
# define PRIi64 "li"
# define PRIu64 "lu"
# define PRIo64 "lo"
# define PRIx64 "lx"
# define PRIX64 "lX"
#endif /* E_CONFIG_C89_LONG_IS_64_BIT */

typedef uint8_t bool;
#define false 0
#define true 1

#endif /* E_STDC_VERSION < E_STDC_VERSION_C99 */

/* miscellaneous **************************************************************/

#if !E_CONFIG_HAS_SYS_TYPES
typedef ptrdiff_t ssize_t;
static_assert (sizeof (isize) == sizeof (usize));
#endif /* !E_CONFIG_HAS_SYS_TYPES */

#endif /* _EMPOWER_BACKPORT_H_ */
