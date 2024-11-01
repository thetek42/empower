#ifndef _EMPOWER_RAND_H_
#define _EMPOWER_RAND_H_

/*! e_rand ********************************************************************
 * 
 * This module provides functions related to randomization.
 *
 ******************************************************************************/

#include <stdint.h>

/* public interface ***********************************************************/

void e_rand_seed (uint64_t seed);
void e_rand_seed_time (void);
uint32_t e_rand_u32 (void);
uint64_t e_rand_u64 (void);
float e_rand_f32 (void);
double e_rand_f64 (void);

/* implementation *************************************************************/

#ifdef E_RAND_IMPL

#include <stdlib.h>
#include <time.h>

/**
 * Seed the pseudo-random number source with the value \seed.
 */
void
e_rand_seed (uint64_t seed)
{
#if defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
	srand ((unsigned int) seed);
#else /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
	srand48 ((long) seed);
#endif /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
}

/**
 * Seed the pseudo-random number source with the result from `time()`. This is
 * not amazing, but works for most cases.
 */
void
e_rand_seed_time (void)
{
	e_rand_seed ((uint64_t) time (NULL));
}

/**
 * Obtain a pseudo-random 32-bit number.
 */
uint32_t
e_rand_u32 (void)
{
#if defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
# if RAND_MAX != 0x7FFF
#  error "RAND_MAX is expected to be 0x7FFF on Windows."
# endif /* RAND_MAX != 0x7FFF */
	return ((uint32_t) rand () << 30) | ((uint32_t) rand () << 15) |
	       ((uint32_t) rand ());
#else /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
	return (uint32_t) mrand48 ();
#endif /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
}

/**
 * Obtain a pseudo-random 64-bit number.
 */
uint64_t
e_rand_u64 (void)
{
#if defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
	return ((uint64_t) rand () << 60) | ((uint64_t) rand () << 45) |
	       ((uint64_t) rand () << 30) | ((uint64_t) rand () << 15) |
	       ((uint64_t) rand ());
#else /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
	uint64_t a, b;
	a = (uint64_t) (uint32_t) mrand48 ();
	b = (uint64_t) (uint32_t) mrand48 ();
	return (a << 32) | b;
#endif /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
}

/**
 * Obtain a pseudo-random single-precision floating point number between 0.0f
 * (inclusive) and 1.0f (exclusive).
 */
float
e_rand_f32 (void)
{
#if defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
	return ((float) rand ()) * (1.0f / RAND_MAX);
#else /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
	return (float) drand48 ();
#endif /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
}

/**
 * Obtain a pseudo-random double-precision floating point number between 0.0
 * (inclusive) and 1.0 (exclusive).
 */
double
e_rand_f64 (void)
{
#if defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
	return ((double) rand ()) * (1.0 / RAND_MAX);
#else /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
	return drand48 ();
#endif /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
}

#endif /* E_RAND_IMPL */

#endif /* _EMPOWER_RAND_H_ */
