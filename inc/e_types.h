#ifndef _EMPOWER_TYPES_H_
#define _EMPOWER_TYPES_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

/*! e_test ********************************************************************
 * 
 * This module provides convenience typedefs.
 *
 ******************************************************************************/

/** docgen: collect
 * Convenience typedefs for fixed-with integer types and sizes.
 */
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
/* docgen: collect-end */

/** docgen: collect
 * Convenience typedefs for traditional C integer types.
 */
typedef signed char ichar;
typedef signed long long llong;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
/* docgen: collect-end */

/** docgen: collect
 * Convenience typedefs for floating point numbers.
 */
typedef float f32;
typedef double f64;
typedef long double f128;
/* docgen: collect-end */

#endif /* _EMPOWER_TYPES_H_ */
