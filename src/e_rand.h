#ifndef _EMPOWER_RAND_H_
#define _EMPOWER_RAND_H_

/*! e_rand ********************************************************************
 * 
 * This module provides functions related to randomization.
 *
 ******************************************************************************/

#include <stdint.h>

/* public interface ***********************************************************/

void e_math_rand_seed (uint64_t seed);
void e_math_rand_seed_time (void);
uint32_t e_math_rand_u32 (void);
uint64_t e_math_rand_u64 (void);
float e_math_rand_f32 (void);
double e_math_rand_f64 (void);

/* implementation *************************************************************/

#ifdef E_RAND_IMPL

#include <stdlib.h>
#include <time.h>

/**
 * Seed the pseudo-random number source with the value \seed.
 */
void
e_math_rand_seed (uint64_t seed)
{
	srand48 ((long) seed);
}

/**
 * Seed the pseudo-random number source with the result from `time()`. This is
 * not amazing, but works for most cases.
 */
void
e_math_rand_seed_time (void)
{
	time_t seed;

	seed = time (NULL);
	srand48 ((long) seed);
}

/**
 * Obtain a pseudo-random 32-bit number.
 */
uint32_t
e_math_rand_u32 (void)
{
	return (uint32_t) mrand48 ();
}

/**
 * Obtain a pseudo-random 64-bit number.
 */
uint64_t
e_math_rand_u64 (void)
{
	uint64_t a, b;
	a = (uint64_t) (uint32_t) mrand48 ();
	b = (uint64_t) (uint32_t) mrand48 ();
	return (a << 32) | b;
}

/**
 * Obtain a pseudo-random single-precision floating point number between 0.0f
 * (inclusive) and 1.0f (exclusive).
 */
float
e_math_rand_f32 (void)
{
	return (float) drand48 ();
}

/**
 * Obtain a pseudo-random double-precision floating point number between 0.0
 * (inclusive) and 1.0 (exclusive).
 */
double
e_math_rand_f64 (void)
{
	return drand48 ();
}

#endif /* E_RAND_IMPL */

#endif /* _EMPOWER_RAND_H_ */
