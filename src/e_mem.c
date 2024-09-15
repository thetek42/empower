#include "empower.h"
#include <stdlib.h>

void *
__e_mem_alloc (usize size)
{
	void *ptr;

	ptr = malloc (size);
	if (ptr == nullptr) {
		e_die ("failed to alloc %zu bytes", size);
	}

	return ptr;
}

void *
__e_mem_calloc (usize nmemb, usize size)
{
	void *ptr;

	ptr = calloc (nmemb, size);
	if (ptr == nullptr) {
		e_die ("failed to calloc %zu bytes", nmemb * size);
	}

	return ptr;
}

void *
__e_mem_realloc (void *ptr, usize size)
{
	ptr = realloc (ptr, size);
	if (ptr == nullptr) {
		e_die ("failed to realloc %zu bytes", size);
	}

	return ptr;
}
