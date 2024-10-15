#include "convey.h"

#if !__C_STDBIT_SUPPORTED

uint64_t
stdc_bit_ceil_u64 (uint64_t x)
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x |= x >> 32;
	x++;
	return x;
}

#endif /* !__C_STDBIT_SUPPORTED */
