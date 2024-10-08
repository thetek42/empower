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

E_ATTR_UNSEQUENCED
bool
e_mem_is_aligned (const void *ptr, usize align)
{
	return ((uintptr_t) ptr % align) == 0;
}

#if E_CONFIG_MODULE_ALLOC

#if E_CONFIG_MODULE_LOG
# define DIE(...) e_die (__VA_ARGS__)
#else /* E_CONFIG_MODULE_LOG */
# define DIE(...)                                                              \
	do {                                                                   \
		fprintf (stderr, "ERROR: " __VA_ARGS__);                       \
		exit (EXIT_FAILURE);                                           \
	} while (0)
#endif /* E_CONFIG_MODULE_LOG */

E_ATTR_NODISCARD ("e_mem_strdup allocates memory which must be freed")
char *
e_mem_strdup (const char *s)
{
	char *res;

	res = strdup (s);
	if (res == nullptr) {
		DIE ("failed to strdup");
	}

	return res;
}

E_ATTR_NODISCARD ("e_mem_strndup allocates memory which must be freed")
char *
e_mem_strndup (const char *s, usize n)
{
	char *res;

	res = strndup (s, n);
	if (res == nullptr) {
		DIE ("failed to strndup");
	}

	return res;
}

E_ATTR_NODISCARD ("e_mem_dup allocates memory which must be freed")
void *
e_mem_dup (const void *ptr, usize n)
{
	void *ret;

	if (!ptr || n == 0) return nullptr;
	ret = e_alloc_size (n);
	memcpy (ret, ptr, n);
	return ret;
}

#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_MEM */
