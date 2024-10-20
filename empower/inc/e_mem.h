#ifndef _EMPOWER_MEM_H_
#define _EMPOWER_MEM_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_MEM

/*! e_mem *********************************************************************
 * 
 * This module provides miscellaneous functions for operating on memory.
 *
 * Module dependencies:
 *  - e_alloc (optional)
 *  - e_log (optional)
 *  - e_debug (optional; transitive)
 *
 ******************************************************************************/

#include <string.h>
#include "convey.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_log.h"

#define e_mem_copy(dest, src, T, n) memcpy ((dest), (src), sizeof (T) * (n))
#define e_mem_copy_size(dest, src, size) memcpy ((dest), (src), (size))
#define e_mem_move(dest, src, T, n) memmove ((dest), (src), sizeof (T) * (n))
#define e_mem_move_size(dest, src, size) memmove ((dest), (src), (size))
#define e_mem_eq(a, b, T, n) e_mem_eq_size ((a), (b), sizeof (T) * (n))
#define e_mem_swap(a, b, T, n) e_mem_swap_size ((a), (b), sizeof (T) * (n))
#define e_mem_is_zero(ptr, T, n) e_mem_is_zero_size ((ptr), sizeof (T) * (n))
#if E_CONFIG_MODULE_ALLOC
# define e_mem_clone(ptr, T, n) e_mem_clone_size ((ptr), sizeof (T) * (n))
#endif /* E_CONFIG_MODULE_ALLOC */
#ifndef C_ALIGNOF_NOT_SUPPORTED
# define e_mem_is_aligned_to_type(ptr, T) e_mem_is_aligned_to ((ptr), alignof (T))
#endif /* C_ALIGNOF_NOT_SUPPORTED */

bool e_mem_is_aligned_to (const void *ptr, usize align);
bool e_mem_eq_size (const void *a, const void *b, usize n);
bool e_mem_is_zero_size (const void *ptr, usize n);
void e_mem_swap_size (void *a, void *b, usize n);

#if E_CONFIG_MODULE_ALLOC
char *e_mem_strdup (const char *s);
void *e_mem_clone_size (const void *ptr, usize n);
#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_MEM */
#endif /* _EMPOWER_MEM_H_ */
