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

#include "convey.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_log.h"

/**
 * Typed `memcpy`. Copies \n instances of type \T from \src to \dest.
 */
#define e_memcpy(dest, src, T, n) memcpy ((dest), (src), sizeof (T) * (n))

/**
 * Typed `memmove`. Moves \n instances of type \T from \src to \dest.
 */
#define e_memmove(dest, src, T, n) memmove ((dest), (src), sizeof (T) * (n))

#if E_CONFIG_MODULE_ALLOC
/**
 * Typed `memdup`. Duplicates \n instances of type \T from \ptr into by
 * allocating some space. If the allocation fails, the programme is terminated
 * and an error is printed.
 */
# define e_memdup(ptr, T, n) __e_mem_dup ((ptr), sizeof (T) * (n))
#endif /* E_CONFIG_MODULE_ALLOC */

bool e_mem_is_zero (const void *ptr, usize n);
bool e_mem_eq (const void *a, const void *b, usize n);
void e_mem_swap (void *a, void *b, usize n);
bool e_mem_is_aligned (const void *ptr, usize align);

#if E_CONFIG_MODULE_ALLOC
char *e_mem_strdup (const char *s);
void *__e_mem_dup (const void *ptr, usize n);
#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_MEM */
#endif /* _EMPOWER_MEM_H_ */
