#ifndef EMPOWER_RAND_H_
#define EMPOWER_RAND_H_

/******************************************************************************
 *
 * This module provides functions related to randomization. For random number
 * generation, the xoshiro256++ algorithm is used. When C11 or above is used, a
 * thread-local PRNG is provided. Note that all PRNGs in this module must be
 * explicitly seeded by the user, otherwise they will always have the same seed.
 *
 ******************************************************************************/

#if !defined (__STDC_VERSION__) || __STDC_VERSION__ < 199901L
# error e_rand requires C99 or newer
#endif

#include <stdint.h>

struct e_rand_state {
	uint64_t s[4];
};

struct e_rand_state e_rand_state_init (void);
void e_rand_state_seed (struct e_rand_state *state, const uint64_t seed[4]);
uint64_t e_rand_state_next (struct e_rand_state *state);
float e_rand_state_f32 (struct e_rand_state *state);
double e_rand_state_f64 (struct e_rand_state *state);

#if __STDC_VERSION__ >= 201112L && !defined (__TINYC__)
void e_rand_seed (uint64_t seed[4]);
uint64_t e_rand_next (void);
float e_rand_f32 (void);
double e_rand_f64 (void);
#endif

/******************************************************************************/

#ifdef E_RAND_IMPL

#if __STDC_VERSION__ >= 201112L && !defined (__TINYC__)
static _Thread_local struct e_rand_state e_rand_thread_local_state_ = {
        .s = {0, 0, 0, 0},
};
#endif

static inline uint64_t e_rand_rotl_ (uint64_t x, int k);

/**
 * Initialise the pseudo-random number generator. It is always initialised to
 * zero, and you will have to seed it yourself.
 */
struct e_rand_state
e_rand_state_init (void)
{
	return (struct e_rand_state) {
		.s = {0, 0, 0, 0},
	};
}

/**
 * Seed the pseudo-random number generator `state` with `seed`.
 */
void
e_rand_state_seed (struct e_rand_state *state,
                   const uint64_t seed[4])
{
	state->s[0] = seed[0];
	state->s[1] = seed[1];
	state->s[2] = seed[2];
	state->s[3] = seed[3];
}

/**
 * Obtain the next value from the pseudo-random number generator `state`.
 */
uint64_t
e_rand_state_next (struct e_rand_state *state)
{
	uint64_t result, t;

	result = e_rand_rotl_ (state->s[0] + state->s[3], 23) + state->s[0];
	t = state->s[1] << 17;
	state->s[2] ^= state->s[0];
	state->s[3] ^= state->s[1];
	state->s[1] ^= state->s[2];
	state->s[0] ^= state->s[3];
	state->s[2] ^= t;
	state->s[3] = e_rand_rotl_ (state->s[3], 45);

	return result;
}

/**
 * Obtain a pseudo-random single-precision floating point number between 0.0F
 * (inclusive) and 1.0F (exclusive).
 */
float
e_rand_state_f32 (struct e_rand_state *state)
{
	return ((float) e_rand_state_next (state)) * (1.0F / (float) UINT64_MAX);
}

/**
 * Obtain a pseudo-random double-precision floating point number between 0.0
 * (inclusive) and 1.0 (exclusive).
 */
double
e_rand_state_f64 (struct e_rand_state *state)
{
	return ((double) e_rand_state_next (state)) * (1.0 / (double) UINT64_MAX);
}

#if __STDC_VERSION__ >= 201112L && !defined (__TINYC__)

/**
 * Seed the thread-local pseudo-random number generator with `seed`.
 */
void
e_rand_seed (uint64_t seed[4])
{
	e_rand_state_seed (&e_rand_thread_local_state_, seed);
}

/**
 * Obtain the next value from the thread-local pseudo-random number generator.
 */
uint64_t
e_rand_next (void)
{
	return e_rand_state_next (&e_rand_thread_local_state_);
}

/**
 * Obtain a single-precision floating point number between 0.0F (inclusive) and
 * 1.0F (exclusive) from the thread-local pseudo-random number generator.
 */
float
e_rand_f32 (void)
{
	return e_rand_state_f32 (&e_rand_thread_local_state_);
}

/**
 * Obtain a double-precision floating point number between 0.0 (inclusive) and
 * 1.0 (exclusive) from the thread-local pseudo-random number generator.
 */
double
e_rand_f64 (void)
{
	return e_rand_state_f64 (&e_rand_thread_local_state_);
}

#endif

static inline uint64_t
e_rand_rotl_ (uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}

#endif /* E_RAND_IMPL */

#endif /* EMPOWER_RAND_H_ */
