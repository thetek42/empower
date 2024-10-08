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

E_ATTR_NODISCARD ("e_mem_alloc allocates memory which must be freed")
void *
__e_mem_alloc (usize size)
{
	void *ptr;

	ptr = malloc (size);
	if (ptr == nullptr) {
		DIE ("failed to alloc %zu bytes", size);
	}

	return ptr;
}

E_ATTR_NODISCARD ("e_mem_calloc allocates memory which must be freed")
void *
__e_mem_calloc (usize nmemb, usize size)
{
	void *ptr;

	ptr = calloc (nmemb, size);
	if (ptr == nullptr) {
		DIE ("failed to calloc %zu bytes", nmemb * size);
	}

	return ptr;
}

E_ATTR_NODISCARD ("e_mem_realloc allocates memory which must be freed")
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

E_ATTR_NODISCARD ("e_mem_strdup allocates memory which must be freed")
char *
e_mem_strdup (const char *s)
{
	char *res;

	res = strdup (s);
	if (res == nullptr) {
		DIE ("failed to strdup");
	}

	return res;
}

E_ATTR_NODISCARD ("e_mem_strndup allocates memory which must be freed")
char *
e_mem_strndup (const char *s, usize n)
{
	char *res;

	res = strndup (s, n);
	if (res == nullptr) {
		DIE ("failed to strdup");
	}

	return res;
}

E_ATTR_NODISCARD ("e_mem_dup allocates memory which must be freed")
void *
e_mem_dup (const void *ptr, usize n)
{
	void *ret;

	if (!ptr || n == 0) return nullptr;
	ret = e_alloc_size (n);
	memcpy (ret, ptr, n);
	return ret;
}

#endif /* E_CONFIG_MODULE_ALLOC */
