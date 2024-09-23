#ifndef _EMPOWER_MATH_H_
#define _EMPOWER_MATH_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_MATH

/*! e_math ********************************************************************
 * 
 * This module provides additional math-related functions. The random number
 * functions provided here use POSIX's random number generator.
 *
 ******************************************************************************/

/**
 * Seed the POSIX pseudo-random number source with the value \seed.
 *
 * @param The seed for the random number generator
 */
void e_math_rand_seed (u32 seed);

/**
 * Seed the POSIX pseudo-random number source with the result from `time()`.
 * This is not amazing, but works for most cases.
 */
void e_math_rand_seed_time (void);

/**
 * Obtain a pseudo-random 32-bit number.
 *
 * @return A pseudo-random number between 0 and UINT32_MAX.
 */
u32 e_math_rand_u32 (void);

/**
 * Obtain a pseudo-random 64-bit number.
 *
 * @return A pseudo-random number between 0 and UINT64_MAX.
 */
u64 e_math_rand_u64 (void);

/**
 * Obtain a pseudo-random single-precision floating point number between 0.0f
 * (inclusive) and 1.0f (exclusive).
 *
 * @return A pseudo-random float in the range [0.0f, 1.0f)
 */
f32 e_math_rand_f32 (void);

/**
 * Obtain a pseudo-random double-precision floating point number between 0.0
 * (inclusive) and 1.0 (exclusive).
 *
 * @return A pseudo-random double in the range [0.0, 1.0)
 */
f64 e_math_rand_f64 (void);

#endif /* E_CONFIG_MODULE_MATH */
#endif /* _EMPOWER_MATH_H_ */
