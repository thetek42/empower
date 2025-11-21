#ifndef EMPOWER_ALLOC_H_
#define EMPOWER_ALLOC_H_

/******************************************************************************
 *
 * This module provides various macros and wrappers around the default C
 * allocators. The allocation of these wrappers never fails: if their
 * underlying call to malloc fails, they will terminate the programme and
 * print an error message.
 *
 * The following example shows how to use the `e_alloc` macros. Note how
 * checking for NULL is not necessary:
 *
 *     Node *ptr;
 *     ptr = e_alloc_zero (Node, 64);      // calloc 64 `Node`s
 *     ptr = e_realloc (ptr, Node, 128);   // realloc to 128 `Node`s
 *     e_free (ptr);                       // free the memory
 *     ptr = e_new (Node);                 // create a single `Node`
 *     e_free (ptr);                       // free the memory
 *
 ******************************************************************************/

#include <stddef.h>

#define e_alloc(type, nmemb) (type *) (e_alloc_size) (sizeof (type) * (nmemb))
#define e_alloc_zero(type, nmemb) (type *) (e_alloc_zero_size) (sizeof (type) * (nmemb))
#define e_realloc(ptr, type, nmemb) (type *) (e_realloc_size) ((ptr), sizeof (type) * (nmemb))
#define e_new(type) e_alloc (type, 1)
#define e_new_zero(type) e_alloc_zero (type, 1)
#define e_free(ptr) (e_free) (ptr)

void *e_alloc_size (size_t size);
void *e_alloc_zero_size (size_t size);
void *e_realloc_size (void *ptr, size_t size);
void e_free (void *ptr);

/******************************************************************************/

#ifdef E_ALLOC_IMPL

#include <stdio.h>
#include <stdlib.h>

#if __STDC_VERSION__ >= 199901L
# define ERROR_MSG(size) "[e_alloc] failed to allocate %zu bytes\n", size
#else
# define ERROR_MSG(size) "[e_alloc] failed to allocate\n"
#endif
#define CHECK_POINTER(ptr, size) do { \
                if ((ptr) == NULL) { \
                        fprintf (stderr, ERROR_MSG (size)); \
                        exit (EXIT_FAILURE); \
                } \
        } while (0)

void *
(e_alloc_size) (size_t size)
{
	void *ptr;
	ptr = malloc (size);
        CHECK_POINTER (ptr, size);
	return ptr;
}

void *
(e_alloc_zero_size) (size_t size)
{
	void *ptr;
	ptr = calloc (size, sizeof (unsigned char));
        CHECK_POINTER (ptr, size);
	return ptr;
}

void *
(e_realloc_size) (void *ptr, size_t size)
{
        void *newptr;

	if (size == 0) {
		free (ptr);
		return NULL;
	}

	newptr = realloc (ptr, size);
        CHECK_POINTER (newptr, size);

	return newptr;
}

void
(e_free) (void *ptr)
{
	free (ptr);
}

#undef CHECK_POINTER
#undef ERROR_MSG

#endif /* E_ALLOC_IMPL */

#endif /* EMPOWER_ALLOC_H_ */
