#ifndef _EMPOWER_RAND_H_
#define _EMPOWER_RAND_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_RAND

/*! e_rand ********************************************************************
 * 
 * This module provides functions related to randomization.
 *
 ******************************************************************************/

/**
 * Seed the POSIX pseudo-random number source with the value \seed.
 */
void e_math_rand_seed (u32 seed);

/**
 * Seed the POSIX pseudo-random number source with the result from `time()`.
 * This is not amazing, but works for most cases.
 */
void e_math_rand_seed_time (void);

/**
 * Obtain a pseudo-random 32-bit number.
 */
u32 e_math_rand_u32 (void);

/**
 * Obtain a pseudo-random 64-bit number.
 */
u64 e_math_rand_u64 (void);

/**
 * Obtain a pseudo-random single-precision floating point number between 0.0f
 * (inclusive) and 1.0f (exclusive).
 */
f32 e_math_rand_f32 (void);

/**
 * Obtain a pseudo-random double-precision floating point number between 0.0
 * (inclusive) and 1.0 (exclusive).
 */
f64 e_math_rand_f64 (void);

#endif /* E_CONFIG_MODULE_RAND */
#endif /* _EMPOWER_RAND_H_ */
