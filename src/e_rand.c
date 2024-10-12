#include "e_rand.h"

#if E_CONFIG_MODULE_RAND

#include <stdlib.h>
#include <time.h>

/**
 * Seed the POSIX pseudo-random number source with the value \seed.
 */
void
e_math_rand_seed (u32 seed)
{
	srand48 ((long) seed);
}

/**
 * Seed the POSIX pseudo-random number source with the result from `time()`.
 * This is not amazing, but works for most cases.
 */
void
e_math_rand_seed_time (void)
{
	time_t seed;

	seed = time (nullptr);
	srand48 ((long) seed);
}

/**
 * Obtain a pseudo-random 32-bit number.
 */
u32
e_math_rand_u32 (void)
{
	return (u32) mrand48 ();
}

/**
 * Obtain a pseudo-random 64-bit number.
 */
u64
e_math_rand_u64 (void)
{
	return ((u64) mrand48 () << 32) | ((u64) mrand48 ());
}

/**
 * Obtain a pseudo-random single-precision floating point number between 0.0f
 * (inclusive) and 1.0f (exclusive).
 */
f32
e_math_rand_f32 (void)
{
	return (f32) drand48 ();
}

/**
 * Obtain a pseudo-random double-precision floating point number between 0.0
 * (inclusive) and 1.0 (exclusive).
 */
f64
e_math_rand_f64 (void)
{
	return drand48 ();
}

#endif /* E_CONFIG_MODULE_RAND */
