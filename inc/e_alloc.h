#ifndef _EMPOWER_ALLOC_H_
#define _EMPOWER_ALLOC_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_ALLOC

/* TODO: make e_log an optional dependency */
#if !E_CONFIG_MODULE_LOG
# error "module e_alloc depends on: e_log"
#endif /* !E_CONFIG_MODULE_LOG */

/*! e_alloc *******************************************************************
 * 
 * This module provides various allocation-related functions. It also provides
 * various macros and wrappers around the default C allocators. The allocation
 * of these wrappers never fails: if their call to malloc fails, they will
 * terminate the programme and print an error message.
 *
 * Module dependencies:
 *  - e_log
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

/**
 * Duplicate a string. This is equivalent to `strdup`, except that it terminates
 * the programme in case the memory allocation fails. This function only returns
 * `nullptr` if \s is `nullptr`. The allocated memory must be freed by the user
 * using `e_free`.
 */
[[nodiscard]]
char *e_alloc_strdup (const char *s);

/**
 * Duplicate up to \n characters of a string \s. This is equivalent to
 * `strndup`, except that it terminates the programme in case the memory
 * allocation fails. This function only returns `nullptr` if \s is `nullptr`.
 * The allocated memory must be freed by the user using `e_free`.
 */
[[nodiscard]]
char *e_alloc_strndup (const char *s, usize n);

[[nodiscard]]
void *__e_mem_alloc (usize size);

[[nodiscard]]
void *__e_mem_calloc (usize nmemb, usize size);

[[nodiscard]]
void *__e_mem_realloc (void *ptr, usize size);

#endif /* E_CONFIG_MODULE_ALLOC */
#endif /* _EMPOWER_ALLOC_H_ */
