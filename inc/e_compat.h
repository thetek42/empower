#ifndef _EMPOWER_COMPAT_H_
#define _EMPOWER_COMPAT_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

/*! e_compat ******************************************************************
 * 
 * This module serves as a utility for backporting certain features to older
 * versions of C. This includes things like certain keywords, functions, integer
 * types. It also provides compatibility for some common types from headers that
 * might not be present on some systems, such as the ones defined by POSIX.
 *
 ******************************************************************************/

/* === c23 features ========================================================= */

#if E_STDC_VERSION < E_STDC_VERSION_C23

/**
 * `nullptr` from C23.
 */
#define nullptr NULL

/* TODO: more stdbit.h */

#if defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG)
# define stdc_bit_ceil(n) __builtin_stdc_bit_ceil (n)
#else /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */
static inline ulong
stdc_bit_ceil (ulong n)
{
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
#if E_CONFIG_LONG_IS_64_BIT
	n |= n >> 32;
#endif /* E_CONFIG_LONG_IS_64_BIT */
	n++;
	return n;
}
#endif /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */

#endif /* E_STDC_VERSION < E_STDC_VERSION_C23 */

/* === c11 features ========================================================= */

#if E_STDC_VERSION < E_STDC_VERSION_C11

/**
 * `static_assert` form C11. Unlike in C23, \msg is not optional.
 *
 * @param expr: The expression to assert at compile time
 * @param msg: The error message (not printed, but visible in error trace)
 */
/* FIXME: do not rely on __LINE__ */
#define static_assert(expr, msg) typedef char E_MACRO_CONCAT(__e_static_assertion_, __LINE__)[(expr) ? 1 : -1]

#endif /* E_STDC_VERSION < E_STDC_VERSION_C11 */

/* === c99 features ========================================================= */

#if E_STDC_VERSION < E_STDC_VERSION_C99

#if E_CONFIG_C89_HAS_RESTRICT
/**
 * `restrict` from C99.
 */
# define restrict __restrict
#else /* E_CONFIG_C89_HAS_RESTRICT */
/**
 * `restrict` from C99.
 *
 * @warn Not supported by the compiler.
 */
# define restrict
#endif /* E_CONFIG_C89_HAS_RESTRICT */

/** docgen: collect
 * Various fixed-width types from C99's `<stdint.h>`.
 */
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long intmax_t;
typedef unsigned long uintmax_t;
typedef ptrdiff_t intptr_t;
typedef size_t uintptr_t;
#if E_CONFIG_C89_INT_IS_32_BIT
typedef signed int int32_t;
typedef unsigned int uint32_t;
#else /* E_CONFIG_C89_INT_IS_32_BIT */
typedef signed long int32_t;
typedef unsigned long uint32_t;
#endif /* E_CONFIG_C89_INT_IS_32_BIT */
#if E_CONFIG_C89_LONG_IS_64_BIT
typedef signed long int64_t;
typedef unsigned long uint64_t;
static_assert (sizeof (int64_t) == 8, "int64_t is not 8 bytes long");
static_assert (sizeof (uint64_t) == 8, "uint64_t is not 8 bytes long");
#endif /* E_CONFIG_C89_LONG_IS_64_BIT */
static_assert (sizeof (int8_t) == 1, "int8_t is not 1 byte long");
static_assert (sizeof (uint8_t) == 1, "uint8_t is not 1 byte long");
static_assert (sizeof (int16_t) == 2, "int16_t is not 2 bytes long");
static_assert (sizeof (uint16_t) == 2, "uint16_t is not 2 bytes long");
static_assert (sizeof (int32_t) == 4, "int32_t is not 4 bytes long");
static_assert (sizeof (uint32_t) == 4, "uint32_t is not 4 bytes long");
static_assert (sizeof (intptr_t) == sizeof (void *), "intptr_t cannot store void *");
static_assert (sizeof (uintptr_t) == sizeof (void *), "uintptr_t cannot store void *");
/* docgen: collect-end */

/** docgen: collect
 * Various macros for formatting fixed-width integer types akin to C99's
 * `<inttypes.h>`.
 */
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
# define PRId32 "d"
# define PRIi32 "i"
# define PRIu32 "u"
# define PRIo32 "o"
# define PRIx32 "x"
# define PRIX32 "X"
#else /* E_CONFIG_C89_INT_IS_32_BIT */
# define PRId32 "ld"
# define PRIi32 "li"
# define PRIu32 "lu"
# define PRIo32 "lo"
# define PRIx32 "lx"
# define PRIX32 "lX"
#endif /* E_CONFIG_C89_INT_IS_32_BIT */
#if E_CONFIG_C89_LONG_IS_64_BIT
# define PRId64 "ld"
# define PRIi64 "li"
# define PRIu64 "lu"
# define PRIo64 "lo"
# define PRIx64 "lx"
# define PRIX64 "lX"
#endif /* E_CONFIG_C89_LONG_IS_64_BIT */
/* docgen: collect-end */

/** docgen: collect
 * `bool` from C99. Represented as an unsigned, 8-bit integer.
 */
typedef uint8_t bool;
#define false 0
#define true 1
/* docgen: collect-end */

#endif /* E_STDC_VERSION < E_STDC_VERSION_C99 */

/* === miscellaneous ========================================================= */

#if !E_CONFIG_HAS_SYS_TYPES
/**
 * `ssize_t` from `<sys/types.h>`.
 */
typedef ptrdiff_t ssize_t;
static_assert (sizeof (isize) == sizeof (usize));
#endif /* !E_CONFIG_HAS_SYS_TYPES */

#endif /* _EMPOWER_COMPAT_H_ */
