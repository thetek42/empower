#include "empower.h"

#if E_CONFIG_MODULE_MEM

E_ATTR_REPRODUCIBLE
bool
e_mem_is_zero (const void *mem, usize len)
{
	const u8 *ptr;
	usize i;

	if (!mem) return true;
	ptr = mem;
	for (i = 0; i < len; i++) {
		if (ptr[i] != 0) {
			return false;
		}
	}

	return true;
}

#endif /* E_CONFIG_MODULE_MEM */
