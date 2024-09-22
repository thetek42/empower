#ifndef _EMPOWER_ALLOC_H_
#define _EMPOWER_ALLOC_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_ALLOC

#if !E_CONFIG_MODULE_LOG
// TODO: make e_log an optional dependency
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
 * Allocates enough memory for \nmemb items of type \type. When the allocation
 * fails, it will print a message and terminate the programme.
 *
 * @param type: A type or expression which works as an argument to `sizeof`
 * @param nmemb: The number of items to allocate space for
 * @return A pointer to the allocated memory
 * @see `malloc`
 */
#define e_alloc(type, nmemb) __e_mem_alloc (sizeof (type) * (nmemb))

/**
 * Allocates \size bytes of memory. When the allocation fails, it will print a
 * message and terminate the programme.
 *
 * @param size: The number of bytes to allocate
 * @return A pointer to the allocated memory
 * @see `malloc`
 */
#define e_alloc_size(size) __e_mem_alloc ((size))

/**
 * Allocates enough memory for \nmemb items of type \type and initialises the
 * memory with zeroes. When the allocation fails, it will print a message and
 * terminate the programme.
 *
 * @param type: A type or expression which works as an argument to `sizeof`
 * @param nmemb: The number of items to allocate space for
 * @return #nonnull void
 * @see `calloc`
 */
#define e_alloc_zero(type, nmemb) __e_mem_calloc ((nmemb), sizeof (type))

/**
 * Allocates \size bytes of memory and initialises the memory with zeroes. When
 * the allocation fails, it will print a message and terminate the programme.
 *
 * @param size: The number of bytes to allocate
 * @return A pointer to the allocated memory
 * @see `calloc`
 */
#define e_alloc_zero_size(size, nmemb) __e_mem_calloc ((nmemb), (size))

/**
 * Reallocates \ptr to enough memory that fits \nmemb items of type \type. When
 * the allocation fails, it will print a message and terminate the programme.
 * When \nmemb is zero, the call is equivalent to \e_free. When \ptr is
 * `nullptr`, the call is equivalent to \e_alloc.
 *
 * @param ptr: The pointer to reallocate
 * @param type: A type or expression which works as an argument to `sizeof`
 * @param nmemb: The number of items to allocate space for
 * @return A pointer to the allocated memory, or `nullptr` if \size is 0
 * @see `realloc`
 */
#define e_realloc(ptr, type, nmemb) __e_mem_realloc ((ptr), sizeof (type) * (nmemb))

/**
 * Reallocates \ptr to \size bytes. When the allocation fails, it will print a
 * message and terminate the programme. When \nmemb is zero, the call is
 * equivalent to \e_free. When \ptr is `nullptr`, the call is equivalent to
 * \e_alloc.
 *
 * @param ptr: The pointer to reallocate
 * @param size: The number of bytes to rellocate
 * @return A pointer to the allocated memory, or `nullptr` if \size is 0
 * @see `realloc`
 */
#define e_realloc_size(ptr, size) __e_mem_realloc ((ptr), (size))

/**
 * Frees the memory pointed to by \ptr.
 *
 * @param ptr: The pointer to the memory that should be freed
 * @return void
 * @see `free`
 */
#define e_free(ptr) free ((ptr))

/**
 * Allocates memory for 1 element of type \type. When the allocation fails, it
 * will print a message and terminate the programme.
 *
 * @param type: A type or expression which works as an argument to `sizeof`
 * @return A pointer to the allocated memory
 * @see `malloc`
 */
#define e_new(type) e_alloc (type, 1)

/**
 * Allocates memory for 1 element of type \type and initialises the memory with
 * zeroes. When the allocation fails, it will print a message and terminate the
 * programme.
 *
 * @param type: A type or expression which works as an argument to `sizeof`
 * @return A pointer to the allocated memory
 * @see `malloc`
 */
#define e_new_zero(type) e_alloc_zero (type, 1)

[[nodiscard]]
void *__e_mem_alloc (usize size);

[[nodiscard]]
void *__e_mem_calloc (usize nmemb, usize size);

[[nodiscard]]
void *__e_mem_realloc (void *ptr, usize size);

#endif /* E_CONFIG_MODULE_ALLOC */
#endif /* _EMPOWER_ALLOC_H_ */
