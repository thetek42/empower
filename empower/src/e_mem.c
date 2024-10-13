#include "e_mem.h"

#if E_CONFIG_MODULE_MEM

#include <string.h>

/**
 * Check if the memory region pointed to by \ptr with size of \n bytes is
 * zeroed. When \ptr is `nullptr` or \n is 0, `true` is returned.
 */
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

/**
 * Check if two memory regions \a and \b with a size of \n bytes are equal. When
 * the memory of \a and \b is equal or if both \a and \b are `nullptr`, `true`
 * is returned. Otherwise, `false` is returned. This is like calling `memcmp`,
 * except that it allows for `nullptr` (which would cause undefined behaviour
 * with `memcmp`).
 */
bool
e_mem_eq (const void *a, const void *b, usize n)
{
	if (a == b) return true;
	if (!a || !b) return false;
	return memcmp (a, b, n) == 0;
}

/**
 * Swaps \n bytes of memory from \a and \b. If \a or \b is `nullptr`, no action
 * is performed.
 */
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

/**
 * Checks if the pointer \ptr is aligned to \align bytes.
 */
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

/**
 * Duplicate a string. This is equivalent to `strdup`, except that it terminates
 * the programme in case the memory allocation fails. This function only returns
 * `nullptr` if \s is `nullptr`. The allocated memory must be freed by the user
 * using `e_free`.
 */
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

/**
 * Duplicate up to \n characters of a string \s. This is equivalent to
 * `strndup`, except that it terminates the programme in case the memory
 * allocation fails. This function only returns `nullptr` if \s is `nullptr`.
 * The allocated memory must be freed by the user using `e_free`.
 */
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

/**
 * Duplicate \n bytes of memory from the pointer \ptr. The memory is allocated
 * using `e_alloc` and must be freed with `e_free`. If \ptr is `nullptr` or \n
 * is 0, no allocation is performed and `nullptr` is returned.
 */
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
