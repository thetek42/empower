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

#endif /* E_CONFIG_MODULE_MEM */
#endif /* _EMPOWER_MEM_H_ */
