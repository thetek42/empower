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
 *  - e_debug (optional; transitive)
 *  - e_log (transitive, optional)
 *
 ******************************************************************************/

bool e_mem_is_zero (const void *ptr, usize n);
bool e_mem_eq (const void *a, const void *b, usize n);
void e_mem_swap (void *a, void *b, usize n);
bool e_mem_is_aligned (const void *ptr, usize align);

#if E_CONFIG_MODULE_ALLOC
char *e_mem_strdup (const char *s);
char *e_mem_strndup (const char *s, usize n);
void *e_mem_dup (const void *ptr, usize n);
#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_MEM */
#endif /* _EMPOWER_MEM_H_ */
