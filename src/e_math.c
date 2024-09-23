#include "empower.h"

void
e_math_rand_seed (u32 seed)
{
	srand48 ((long) seed);
}

void
e_math_rand_seed_time (void)
{
	time_t seed;

	seed = time (nullptr);
	srand48 ((long) seed);
}

u32
e_math_rand_u32 (void)
{
	return (u32) mrand48 ();
}

u64
e_math_rand_u64 (void)
{
	return ((u64) mrand48 () << 32) | ((u64) mrand48 ());
}

f32
e_math_rand_f32 (void)
{
	return (f32) drand48 ();
}

f64
e_math_rand_f64 (void)
{
	return drand48 ();
}
