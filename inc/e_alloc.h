#ifndef _EMPOWER_ALLOC_H_
#define _EMPOWER_ALLOC_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_ALLOC

#if !defined (E_CONFIG_MODULE_DEBUG)
# error "module e_log depends on: e_debug"
#endif /* !defined (E_CONFIG_MODULE_DEBUG) */

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
 ******************************************************************************/

/**
 * Allocates enough memory for \nmemb items of type \type using `malloc`. When
 * the allocation fails, it will print a message and terminate the programme.
 */
#define e_alloc(type, nmemb) __e_mem_alloc (sizeof (type) * (nmemb))

/**
 * Allocates \size bytes of memory using `malloc`. When the allocation fails, it
 * will print a message and terminate the programme.
 */
#define e_alloc_size(size) __e_mem_alloc ((size))

/**
 * Allocates enough memory for \nmemb items of type \type using `calloc` and
 * initialises the memory with zeroes. When the allocation fails, it will print
 * a message and terminate the programme.
 */
#define e_alloc_zero(type, nmemb) __e_mem_calloc ((nmemb), sizeof (type))

/**
 * Allocates \size bytes of memory using `calloc` and initialises the memory
 * with zeroes. When the allocation fails, it will print a message and terminate
 * the programme.
 */
#define e_alloc_zero_size(size, nmemb) __e_mem_calloc ((nmemb), (size))

/**
 * Reallocates \ptr to enough memory that fits \nmemb items of type \type using
 * `realloc`. When the allocation fails, it will print a message and terminate
 * the programme. When \nmemb is zero, the call is equivalent to `e_free` and
 * the function returns `nullptr`. When \ptr is `nullptr`, the call is
 * equivalent to `e_alloc`.
 */
#define e_realloc(ptr, type, nmemb) __e_mem_realloc ((ptr), sizeof (type) * (nmemb))

/**
 * Reallocates \ptr to \size bytes using `realloc`. When the allocation fails,
 * it will print a message and terminate the programme. When \size is zero, the
 * call is equivalent to `e_free` and the function returns `nullptr`. When \ptr
 * is `nullptr`, the call is equivalent to `e_alloc`.
 */
#define e_realloc_size(ptr, size) __e_mem_realloc ((ptr), (size))

/**
 * Frees the memory pointed to by \ptr using `free`.
 */
#define e_free(ptr) free ((ptr))

/**
 * Allocates memory for 1 element of type \type using `malloc`. When the
 * allocation fails, it will print a message and terminate the programme.
 */
#define e_new(type) e_alloc (type, 1)

/**
 * Allocates memory for 1 element of type \type using `calloc` and initialises
 * the memory with zeroes. When the allocation fails, it will print a message
 * and terminate the programme.
 */
#define e_new_zero(type) e_alloc_zero (type, 1)

E_ATTR_NODISCARD ("e_mem_alloc allocates memory which must be freed")
void *__e_mem_alloc (usize size);

E_ATTR_NODISCARD ("e_mem_calloc allocates memory which must be freed")
void *__e_mem_calloc (usize nmemb, usize size);

E_ATTR_NODISCARD ("e_mem_realloc allocates memory which must be freed")
void *__e_mem_realloc (void *ptr, usize size);

#endif /* E_CONFIG_MODULE_ALLOC */
#endif /* _EMPOWER_ALLOC_H_ */
