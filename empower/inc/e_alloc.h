#ifndef _EMPOWER_ALLOC_H_
#define _EMPOWER_ALLOC_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_ALLOC

/*! e_alloc *******************************************************************
 * 
 * This module provides various allocation-related functions. It also provides
 * various macros and wrappers around the default C allocators. The allocation
 * of these wrappers never fails: if their call to malloc fails, they will
 * terminate the programme and print an error message.
 *
 * Module dependencies:
 *  - e_debug
 *  - e_log (optional)
 *
 * The following example shows how to use the `e_alloc` macros. Note how
 * checking for `nullptr` is not necessary:
 *  | Node *ptr;
 *  | ptr = e_alloc_zero (Node, 64);      // calloc 64 `Node`s
 *  | ptr = e_realloc (ptr, Node, 128);   // realloc to 128 `Node`s
 *  | e_free (ptr);                       // free the memory
 *  | ptr = e_new (Node);                 // create a single `Node`
 *
 ******************************************************************************/

#if !defined (E_CONFIG_MODULE_DEBUG)
# error "module e_log depends on: e_debug"
#endif /* !defined (E_CONFIG_MODULE_DEBUG) */

#include <stdlib.h>
#include "convey.h"
#include "e_types.h"
#include "e_debug.h"
#include "e_log.h"

#define e_alloc(type, nmemb) __e_mem_alloc (sizeof (type) * (nmemb))
#define e_alloc_size(size) __e_mem_alloc ((size))
#define e_alloc_zero(type, nmemb) __e_mem_calloc ((nmemb), sizeof (type))
#define e_alloc_zero_size(size, nmemb) __e_mem_calloc ((nmemb), (size))
#define e_realloc(ptr, type, nmemb) __e_mem_realloc ((ptr), sizeof (type) * (nmemb))
#define e_realloc_size(ptr, size) __e_mem_realloc ((ptr), (size))
#define e_new(type) e_alloc (type, 1)
#define e_new_zero(type) e_alloc_zero (type, 1)
#define e_free(ptr) free ((ptr))

void *__e_mem_alloc (usize size);
void *__e_mem_calloc (usize nmemb, usize size);
void *__e_mem_realloc (void *ptr, usize size);

#endif /* E_CONFIG_MODULE_ALLOC */
#endif /* _EMPOWER_ALLOC_H_ */
