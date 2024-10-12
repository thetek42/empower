#include "empower.h"
#include <stdlib.h>

#if E_CONFIG_MODULE_LOG
# define DIE(...) e_die (__VA_ARGS__)
#else /* E_CONFIG_MODULE_LOG */
# define DIE(...)                                                              \
	do {                                                                   \
		fprintf (stderr, "ERROR: " __VA_ARGS__);                       \
		exit (EXIT_FAILURE);                                           \
	} while (0)
#endif /* E_CONFIG_MODULE_LOG */

#if E_CONFIG_MODULE_ALLOC

void *
__e_mem_alloc (usize size)
{
	void *ptr;

	e_debug_assert (size != 0, "size for e_alloc cannot be 0");

	ptr = malloc (size);
	if (ptr == nullptr) {
		DIE ("failed to alloc %zu bytes", size);
	}

	return ptr;
}

void *
__e_mem_calloc (usize nmemb, usize size)
{
	void *ptr;

	e_debug_assert (size != 0, "size for e_calloc cannot be 0");

	ptr = calloc (nmemb, size);
	if (ptr == nullptr) {
		DIE ("failed to calloc %zu bytes", nmemb * size);
	}

	return ptr;
}

void *
__e_mem_realloc (void *ptr, usize size)
{
	if (size == 0) {
		free (ptr);
		return nullptr;
	}

	ptr = realloc (ptr, size);
	if (ptr == nullptr) {
		DIE ("failed to realloc %zu bytes", size);
	}

	return ptr;
}

#endif /* E_CONFIG_MODULE_ALLOC */
