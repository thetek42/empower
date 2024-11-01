#ifndef _EMPOWER_MEM_H_
#define _EMPOWER_MEM_H_

/*! e_mem *********************************************************************
 * 
 * This module provides miscellaneous functions for operating on memory.
 *
 ******************************************************************************/

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>

/* public interface ***********************************************************/

#define e_mem_copy(dest, src, T, n) memcpy ((dest), (src), sizeof (T) * (n))
#define e_mem_copy_size(dest, src, size) memcpy ((dest), (src), (size))
#define e_mem_move(dest, src, T, n) memmove ((dest), (src), sizeof (T) * (n))
#define e_mem_move_size(dest, src, size) memmove ((dest), (src), (size))
#define e_mem_eq(a, b, T, n) e_mem_eq_size ((a), (b), sizeof (T) * (n))
#define e_mem_swap(a, b, T, n) e_mem_swap_size ((a), (b), sizeof (T) * (n))
#define e_mem_is_zero(ptr, T, n) e_mem_is_zero_size ((ptr), sizeof (T) * (n))
#define e_mem_clone(ptr, T, n) e_mem_clone_size ((ptr), sizeof (T) * (n))
#define e_mem_is_aligned_to_type(ptr, T) e_mem_is_aligned_to ((ptr), alignof (T))

bool e_mem_is_aligned_to (const void *ptr, size_t align);
bool e_mem_eq_size (const void *a, const void *b, size_t n);
bool e_mem_is_zero_size (const void *ptr, size_t n);
void e_mem_swap_size (void *a, void *b, size_t n);
char *e_mem_strdup (const char *s);
void *e_mem_clone_size (const void *ptr, size_t n);

/* implementation *************************************************************/

#ifdef E_MEM_IMPL

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Check if the memory region pointed to by \ptr with size of \n bytes is
 * zeroed. When \ptr is `nullptr` or \n is 0, `true` is returned.
 */
bool
e_mem_is_zero_size (const void *ptr, size_t n)
{
	const uint8_t *p;
	size_t i;

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
e_mem_eq_size (const void *a, const void *b, size_t n)
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
e_mem_swap_size (void *a, void *b, size_t n)
{
	uint8_t *ap, *bp, tmp;
	size_t i;

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
e_mem_is_aligned_to (const void *ptr, size_t align)
{
	return ((uintptr_t) ptr % align) == 0;
}

/**
 * Duplicate a string. This is equivalent to `strdup`, except that it terminates
 * the programme in case the memory allocation fails. This function only returns
 * `nullptr` if \s is `nullptr`. The allocated memory must be freed by the user
 * using `e_free`.
 */
char *
e_mem_strdup (const char *s)
{
	size_t len;
	char *res;

	if (!s) return NULL;
	len = strlen (s);
	res = malloc (sizeof (char) * (len + 1));
	if (res == NULL) {
		fprintf (stderr, "[e_mem] failed to strdup\n");
		exit (EXIT_FAILURE);
	}
	strcpy (res, s);

	return res;
}

/**
 * Duplicate \n bytes of memory from the pointer \ptr. The memory is allocated
 * using `e_alloc` and must be freed with `e_free`. If \ptr is `nullptr` or \n
 * is 0, no allocation is performed and `nullptr` is returned.
 */
void *
e_mem_clone_size (const void *ptr, size_t n)
{
	void *ret;

	if (!ptr || n == 0) return NULL;
	ret = malloc (n);
	if (ret == NULL) {
		fprintf (stderr, "[e_mem] failed to alloc %zu bytes\n", n);
		exit (EXIT_FAILURE);
	}
	memcpy (ret, ptr, n);
	return ret;
}

#endif /* E_MEM_IMPL */

#endif /* _EMPOWER_MEM_H_ */
