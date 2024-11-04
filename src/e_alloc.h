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
 ******************************************************************************/

#include <stddef.h>
#include <stdlib.h>

/* public interface ***********************************************************/

#define e_alloc(type, nmemb) e_alloc_size (sizeof (type) * (nmemb))
#define e_alloc_zero(type, nmemb) (e_alloc_zero_size) ((nmemb), sizeof (type))
#define e_alloc_zero_size(size) (e_alloc_zero_size) ((size), sizeof (char))
#define e_realloc(ptr, type, nmemb) e_realloc_size ((ptr), sizeof (type) * (nmemb))
#define e_new(type) e_alloc (type, 1)
#define e_new_zero(type) e_alloc_zero (type, 1)
#define e_free(ptr) free ((ptr))

void *e_alloc_size (size_t size);
void *(e_alloc_zero_size) (size_t nmemb, size_t size);
void *e_realloc_size (void *ptr, size_t size);

/* implementation *************************************************************/

#ifdef E_ALLOC_IMPL

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void *
e_alloc_size (size_t size)
{
	void *ptr;

	assert (size != 0 && "size for e_alloc cannot be 0");

	ptr = malloc (size);
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

	ptr = calloc (nmemb, size);
	if (ptr == NULL) {
		fprintf (stderr, "[e_alloc] failed to alloc %zu bytes\n", size);
		exit (EXIT_FAILURE);
	}

	return ptr;
}

void *
e_realloc_size (void *ptr, size_t size)
{
	if (size == 0) {
		free (ptr);
		return NULL;
	}

	ptr = realloc (ptr, size);
	if (ptr == NULL) {
		fprintf (stderr, "[e_alloc] failed to alloc %zu bytes\n", size);
		exit (EXIT_FAILURE);
	}

	return ptr;
}

#endif /* E_ALLOC_IMPL */

#endif /* _EMPOWER_ALLOC_H_ */
