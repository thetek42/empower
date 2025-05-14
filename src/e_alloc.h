#ifndef _EMPOWER_ALLOC_H_
#define _EMPOWER_ALLOC_H_

/*! e_alloc *******************************************************************
 *
 * This module provides various allocation-related functions. It also provides
 * various macros and wrappers around the default C allocators. The allocation
 * of these wrappers never fails: if their call to malloc fails, they will
 * terminate the programme and print an error message.
 *
 * The following example shows how to use the `e_alloc` macros. Note how
 * checking for `nullptr` is not necessary:
 *
 *  | Node *ptr;
 *  | ptr = e_alloc_zero (Node, 64);      // calloc 64 `Node`s
 *  | ptr = e_realloc (ptr, Node, 128);   // realloc to 128 `Node`s
 *  | e_free (ptr);                       // free the memory
 *  | ptr = e_new (Node);                 // create a single `Node`
 *
 * Configuration options:
 *  - `E_CONFIG_MALLOC_FUNC` (default `malloc`): The function to use for allocating memory.
 *  - `E_CONFIG_CALLOC_FUNC` (default `calloc`): The function to use for allocating zeroed memory.
 *  - `E_CONFIG_REALLOC_FUNC` (default `realloc`): The function to use for reallocating memory.
 *  - `E_CONFIG_FREE_FUNC` (default `free`): The function to use for freeing memory.
 *
 ******************************************************************************/

#include <stddef.h>

/* public interface ***********************************************************/

#define e_alloc(type, nmemb) (type *) (e_alloc_size) (sizeof (type) * (nmemb))
#define e_alloc_size(size) (e_alloc_size) (size)
#define e_alloc_zero(type, nmemb) (type *) (e_alloc_zero_size) ((nmemb), sizeof (type))
#define e_alloc_zero_size(size) (e_alloc_zero_size) ((size), sizeof (char))
#define e_realloc(ptr, type, nmemb) (type *) (e_realloc_size) ((ptr), sizeof (type) * (nmemb))
#define e_realloc_size(ptr, size) (e_realloc_size) ((ptr), (size))
#define e_new(type) e_alloc (type, 1)
#define e_new_zero(type) e_alloc_zero (type, 1)
#define e_free(ptr) (e_free) (ptr)

void *(e_alloc_size) (size_t size);
void *(e_alloc_zero_size) (size_t nmemb, size_t size);
void *(e_realloc_size) (void *ptr, size_t size);
void (e_free) (void *ptr);

/* implementation *************************************************************/

#ifdef E_ALLOC_IMPL

#ifndef E_CONFIG_MALLOC_FUNC
# define E_CONFIG_MALLOC_FUNC malloc
#endif /* E_CONFIG_MALLOC_FUNC */
#ifndef E_CONFIG_CALLOC_FUNC
# define E_CONFIG_CALLOC_FUNC calloc
#endif /* E_CONFIG_CALLOC_FUNC */
#ifndef E_CONFIG_REALLOC_FUNC
# define E_CONFIG_REALLOC_FUNC realloc
#endif /* E_CONFIG_REALLOC_FUNC */
#ifndef E_CONFIG_FREE_FUNC
# define E_CONFIG_FREE_FUNC free
#endif /* E_CONFIG_FREE_FUNC */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void *
(e_alloc_size) (size_t size)
{
	void *ptr;

	assert (size != 0 && "size for e_alloc cannot be 0");

	ptr = E_CONFIG_MALLOC_FUNC (size);
	if (ptr == NULL) {
		fprintf (stderr, "[e_alloc] failed to alloc %zu bytes\n", size);
		exit (EXIT_FAILURE);
	}

	return ptr;
}

void *
(e_alloc_zero_size) (size_t nmemb, size_t size)
{
	void *ptr;

	assert (size != 0 && "size for e_calloc cannot be 0");

	ptr = E_CONFIG_CALLOC_FUNC (nmemb, size);
	if (ptr == NULL) {
		fprintf (stderr, "[e_alloc] failed to alloc %zu bytes\n", size);
		exit (EXIT_FAILURE);
	}

	return ptr;
}

void *
(e_realloc_size) (void *ptr, size_t size)
{
	if (size == 0) {
		E_CONFIG_FREE_FUNC (ptr);
		return NULL;
	}

	ptr = E_CONFIG_REALLOC_FUNC (ptr, size);
	if (ptr == NULL) {
		fprintf (stderr, "[e_alloc] failed to alloc %zu bytes\n", size);
		exit (EXIT_FAILURE);
	}

	return ptr;
}

void
(e_free) (void *ptr)
{
	E_CONFIG_FREE_FUNC (ptr);
}

#endif /* E_ALLOC_IMPL */

#endif /* _EMPOWER_ALLOC_H_ */
