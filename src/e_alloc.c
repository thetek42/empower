#include "empower.h"
#include <stdlib.h>

#if E_CONFIG_MODULE_ALLOC

E_ATTR_ALLOC_SIZE (1)
E_ATTR_MALLOC_WITH_DEALLOC (free)
E_ATTR_RETURNS_NONNULL
void *
__e_mem_alloc (usize size)
{
	void *ptr;

	ptr = malloc (size);
	if (ptr == nullptr) {
		e_die ("failed to alloc %"E_FMT_USIZE" bytes", size);
	}

	return ptr;
}

E_ATTR_ALLOC_SIZE_MULT (1, 2)
E_ATTR_MALLOC_WITH_DEALLOC (free)
E_ATTR_RETURNS_NONNULL
void *
__e_mem_calloc (usize nmemb, usize size)
{
	void *ptr;

	ptr = calloc (nmemb, size);
	if (ptr == nullptr) {
		e_die ("failed to calloc %"E_FMT_USIZE" bytes", nmemb * size);
	}

	return ptr;
}

E_ATTR_ALLOC_SIZE (2)
E_ATTR_MALLOC_WITH_DEALLOC (free)
void *
__e_mem_realloc (void *ptr, usize size)
{
	if (size == 0) {
		free (ptr);
		return nullptr;
	}

	ptr = realloc (ptr, size);
	if (ptr == nullptr) {
		e_die ("failed to realloc %"E_FMT_USIZE" bytes", size);
	}

	return ptr;
}

#endif /* E_CONFIG_MODULE_ALLOC */
