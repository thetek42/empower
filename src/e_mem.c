#include "empower.h"

#if E_CONFIG_MODULE_MEM

E_ATTR_REPRODUCIBLE
bool
e_mem_is_zero (const void *ptr, usize n)
{
	const u8 *p;
	usize i;

	if (!ptr) return true;
	p = ptr;
	for (i = 0; i < n; i++) {
		if (p[i] != 0) {
			return false;
		}
	}

	return true;
}

E_ATTR_REPRODUCIBLE
bool
e_mem_eq (const void *a, const void *b, usize n)
{
	if (a == b) return true;
	if (!a || !b) return false;
	return memcmp (a, b, n) == 0;
}

E_ATTR_REPRODUCIBLE
void
e_mem_swap (void *a, void *b, usize n)
{
	u8 *ap, *bp, tmp;
	usize i;

	if (!a || !b) return;
	ap = a;
	bp = b;
	for (i = 0; i < n; i++) {
		tmp = ap[i];
		ap[i] = bp[i];
		bp[i] = tmp;
	}
}

#endif /* E_CONFIG_MODULE_MEM */
