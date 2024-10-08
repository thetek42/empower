#ifndef _EMPOWER_MEM_H_
#define _EMPOWER_MEM_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_MEM

/*! e_mem *********************************************************************
 * 
 * This module provides miscellaneous functions for operating on memory.
 *
 * Module dependencies:
 *  - e_alloc (optional)
 *  - e_log (transitive, optional)
 *
 ******************************************************************************/

/**
 * Check if the memory region pointed to by \ptr with size of \n bytes is
 * zeroed. When \ptr is `nullptr` or \n is 0, `true` is returned.
 */
E_ATTR_REPRODUCIBLE
bool e_mem_is_zero (const void *ptr, usize n);

/**
 * Check if two memory regions \a and \b with a size of \n bytes are equal. When
 * the memory of \a and \b is equal or if both \a and \b are `nullptr`, `true`
 * is returned. Otherwise, `false` is returned. This is like calling `memcmp`,
 * except that it allows for `nullptr` (which would cause undefined behaviour
 * with `memcmp`).
 */
E_ATTR_REPRODUCIBLE
bool e_mem_eq (const void *a, const void *b, usize n);

/**
 * Swaps \n bytes of memory from \a and \b. If \a or \b is `nullptr`, no action
 * is performed.
 */
E_ATTR_REPRODUCIBLE
void e_mem_swap (void *a, void *b, usize n);

/**
 * Checks if the pointer \ptr is aligned to \align bytes.
 */
E_ATTR_UNSEQUENCED
bool e_mem_is_aligned (const void *ptr, usize align);

#if E_CONFIG_MODULE_ALLOC

/**
 * Duplicate a string. This is equivalent to `strdup`, except that it terminates
 * the programme in case the memory allocation fails. This function only returns
 * `nullptr` if \s is `nullptr`. The allocated memory must be freed by the user
 * using `e_free`.
 */
E_ATTR_NODISCARD ("e_mem_strdup allocates memory which must be freed")
char *e_mem_strdup (const char *s);

/**
 * Duplicate up to \n characters of a string \s. This is equivalent to
 * `strndup`, except that it terminates the programme in case the memory
 * allocation fails. This function only returns `nullptr` if \s is `nullptr`.
 * The allocated memory must be freed by the user using `e_free`.
 */
E_ATTR_NODISCARD ("e_mem_strndup allocates memory which must be freed")
char *e_mem_strndup (const char *s, usize n);

/**
 * Duplicate \n bytes of memory from the pointer \ptr. The memory is allocated
 * using `e_alloc` and must be freed with `e_free`. If \ptr is `nullptr` or \n
 * is 0, no allocation is performed and `nullptr` is returned.
 */
E_ATTR_NODISCARD ("e_mem_dup allocates memory which must be freed")
void *e_mem_dup (const void *ptr, usize n);

#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_MEM */
#endif /* _EMPOWER_MEM_H_ */
