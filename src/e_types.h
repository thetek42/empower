#ifndef _EMPOWER_TYPES_H_
#define _EMPOWER_TYPES_H_

/*! e_types *******************************************************************
 * 
 * This module provides convenience typedefs.
 *
 ******************************************************************************/

#include <stddef.h>
#include <stdint.h>

/* compatibility annoyances ***************************************************/

#if defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
# if !defined (__E_SSIZE_T_DEFINED)
#  define __E_SSIZE_T_DEFINED
#  include <basetsd.h>
typedef SSIZE_T ssize_t;
# endif /* !defined (__E_SSIZE_T_DEFINED) */
#else /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
# include <sys/types.h>
#endif /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */

/* public interface ***********************************************************/

#if __STDC_VERSION__ < 202000L || defined (_MSC_VER)
# define nullptr NULL
typedef void *nullptr_t;
#endif /* __STDC_VERSION__ < 202000L || defined (_MSC_VER) */

typedef int8_t i8;
typedef int16_t i16;
typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef size_t usize;
typedef ssize_t isize;
typedef signed char ichar;
typedef signed long long llong;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef float f32;
typedef double f64;
typedef long double f128;

#endif /* _EMPOWER_TYPES_H_ */
