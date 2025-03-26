#ifndef _EMPOWER_DEBUG_H_
#define _EMPOWER_DEBUG_H_

/*! e_debug *******************************************************************
 *
 * This module provides utilities for quick debugging.
 *
 * Module dependencies:
 *  - e_log (optional)
 *
 * Configuration options:
 *  - `E_CONFIG_MALLOC_FUNC` (default `malloc`): The function to use for allocating memory.
 *  - `E_CONFIG_CALLOC_FUNC` (default `calloc`): The function to use for allocating zeroed memory.
 *  - `E_CONFIG_REALLOC_FUNC` (default `realloc`): The function to use for reallocating memory.
 *  - `E_CONFIG_FREE_FUNC` (default `free`): The function to use for freeing memory.
 *
 ******************************************************************************/

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* public interface ***********************************************************/

#define __E_DEBUG_STRINGIFY_HELPER(x) #x
#define __E_DEBUG_STRINGIFY(x) __E_DEBUG_STRINGIFY_HELPER (x)
#define __E_DEBUG_FILE_LINE __FILE__ ":" __E_DEBUG_STRINGIFY (__LINE__)

#if __STDC_VERSION__ >= 201112L

/**
 * Obtain the formatting argument for `printf`-like functions for the generic
 * argument \value. Works for regular integers, floats and pointers.
 */
# define E_DEBUG_AUTO_FMT(value) _Generic ((value),                            \
		int8_t: "%" PRIi8,                                             \
		int16_t: "%" PRIi16,                                           \
		int32_t: "%" PRIi32,                                           \
		int64_t: "%" PRIi64,                                           \
		uint8_t: "%" PRIu8,                                            \
		uint16_t: "%" PRIu16,                                          \
		uint32_t: "%" PRIu32,                                          \
		uint64_t: "%" PRIu64,                                          \
		float: "%f",                                                   \
		double: "%g",                                                  \
		long double: "%lg",                                            \
		bool: "%d",                                                    \
		default: "%p")

/**
 * Debug-print the generic argument \value. For a call like `e_debug (x + y)`,
 * it will print `x + y = 42` with the regular debug format from e_log, where 42
 * is the result of the addition. Works with every type that is accepted by
 * `E_DEBUG_AUTO_FMT`.
 */
# define e_debug(value)                                                        \
	do {                                                                   \
		fprintf (stderr, "[debug] " __E_DEBUG_STRINGIFY (value) " = ");\
		fprintf (stderr, E_DEBUG_AUTO_FMT(value), (value));            \
		fprintf (stderr, " (" __E_DEBUG_FILE_LINE ")\n");              \
	} while (0)

#endif /* __STDC_VERSION__ >= 201112L */

/**
 * This is the same as `assert()`, but with slightly better error reporting.
 */
#ifdef NDEBUG
# define e_assert(expr, msg)
#else /* NDEBUG */
# define e_assert(expr, msg)                                                   \
	do {                                                                   \
		if (!(expr)) {                                                 \
			fprintf (stderr, "Assertion \"%s\" (at "               \
			         __E_DEBUG_FILE_LINE ") failed: " msg "\n",    \
			         #expr);                                       \
		}                                                              \
	} while (0)
#endif /* NDEBUG */

#define e_debug_alloc(type, nmemb) ((e_debug_alloc_size) (sizeof (type) * (nmemb), __E_DEBUG_FILE_LINE))
#define e_debug_alloc_size(size) ((e_debug_alloc_size) ((size), __E_DEBUG_FILE_LINE))
#define e_debug_alloc_zero(type, nmemb) ((e_debug_alloc_zero_size) ((nmemb), sizeof (type), __E_DEBUG_FILE_LINE))
#define e_debug_alloc_zero_size(size) ((e_debug_alloc_zero_size) ((size), sizeof (char), __E_DEBUG_FILE_LINE))
#define e_debug_realloc(ptr, type, nmemb) ((e_debug_realloc_size) ((ptr), sizeof (type) * (nmemb), __E_DEBUG_FILE_LINE))
#define e_debug_realloc_size(ptr, size) ((e_debug_realloc_size) ((ptr), (size), __E_DEBUG_FILE_LINE))
#define e_debug_new(type) ((e_debug_alloc_size) (sizeof (type), __E_DEBUG_FILE_LINE))
#define e_debug_new_zero(type) ((e_debug_alloc_zero_size) (sizeof (type), __E_DEBUG_FILE_LINE))
#define e_debug_free(ptr) ((e_debug_free) ((ptr), __E_DEBUG_FILE_LINE))

#if __STDC_VERSION__ >= 202000L && !defined (_MSC_VER)
# define e_unreachable() unreachable ()
#elif defined (__GNUC__) || defined (__clang__) /* __STDC_VERSION__ >= 202000L && !defined (_MSC_VER) */
# define e_unreachable() __builtin_unreachable ()
#elif defined (_MSC_VER) /* defined (__GNUC__) || defined (__clang__) */
# define e_unreachable() __assume (0)
#else /* defined (_MSC_VER) */
# error "unsupported compilier"
#endif /* defined (_MSC_VER) */

void e_debug_hexdump (const void *ptr, size_t len);
void *(e_debug_alloc_size) (size_t size, const char *location);
void *(e_debug_alloc_zero_size) (size_t nmemb, size_t size, const char *location);
void *(e_debug_realloc_size) (void *ptr, size_t size, const char *location);
void (e_debug_free) (void *ptr, const char *location);

/******************************************************************************/

#ifdef E_DEBUG_IMPL

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/**
 * Print a hexdump of a region of memory.
 */
void
e_debug_hexdump (const void *ptr, size_t len)
{
	const uint8_t *p = ptr;
	size_t i, j, k, l;
	char buf[70] = {0};
	uint8_t c;

	fprintf (stderr, "\n---=== hexdump %p ===---\n\n", (void *) p);

	if (p == NULL) {
		fprintf (stderr, "nullptr\n\n");
		return;
	}

	if (len == 0) {
		fprintf (stderr, "len = 0\n\n");
		return;
	}

	for (i = 0; i < len; i += 16) {
		memset (buf, ' ', 69);
		snprintf (buf, 9, "%08" PRIx32, (uint32_t) i);
		buf[8] = ':';
		for (j = 0; j < 16; j += 1) {
			if (i + j >= len) break;
			c = p[i + j];
			k = 10 + j * 2 + j / 2;
			l = 51 + j + j / 8;
			buf[k] = (char) (c / 16 >= 10 ? c / 16 + 87 : c / 16 + 48);
			buf[k + 1] = (char) (c % 16 >= 10 ? c % 16 + 87 : c % 16 + 48);
			buf[l] = 32 <= c && c <= 126 ? c : '.';
		}
		fprintf (stderr, "%s\n", buf);
	}
	fputc ('\n', stderr);
}

void *
(e_debug_alloc_size) (size_t size, const char *location)
{
	void *ptr;

	assert (size != 0 && "size for e_alloc cannot be 0");

	ptr = E_CONFIG_MALLOC_FUNC (size);
	if (ptr == NULL) {
		fprintf (stderr, "[e_debug] \x1b[34malloc\x1b[0m %zu bytes: \x1b[31mFAILED\x1b[0m \x1b[90m(%s)\x1b[0m\n", size, location);
		exit (EXIT_FAILURE);
	}
	memset (ptr, 0xCD, size);
	fprintf (stderr, "[e_debug] \x1b[34malloc\x1b[0m %zu bytes: \x1b[32m%p\x1b[0m \x1b[90m(%s)\x1b[0m\n", size, ptr, location);

	return ptr;
}

void *
(e_debug_alloc_zero_size) (size_t nmemb, size_t size, const char *location)
{
	void *ptr;

	assert (size != 0 && "size for e_alloc cannot be 0");

	ptr = E_CONFIG_CALLOC_FUNC (nmemb, size);
	if (ptr == NULL) {
		fprintf (stderr, "[e_debug] \x1b[34malloc_zero\x1b[0m %zu bytes: \x1b[31mFAILED\x1b[0m \x1b[90m(%s)\x1b[0m\n", size, location);
		exit (EXIT_FAILURE);
	}
	fprintf (stderr, "[e_debug] \x1b[34malloc_zero\x1b[0m %zu bytes: \x1b[32m%p\x1b[0m \x1b[90m(%s)\x1b[0m\n", size, ptr, location);

	return ptr;
}

void *
(e_debug_realloc_size) (void *ptr, size_t size, const char *location)
{
	uintptr_t old_ptr;

	assert (size != 0 && "size for e_alloc cannot be 0");

	if (size == 0) {
		fprintf (stderr, "[e_debug] \x1b[35mrealloc\x1b[0m free \x1b[33m%p\x1b[0m (size == 0) \x1b[90m(%s)\x1b[0m\n", ptr, location);
		E_CONFIG_FREE_FUNC (ptr);
		return NULL;
	}

	old_ptr = (uintptr_t) ptr;
	ptr = E_CONFIG_REALLOC_FUNC (ptr, size);
	if (ptr == NULL) {
		fprintf (stderr, "[e_debug] \x1b[36mrealloc\x1b[0m %p to %zu bytes: \x1b[31mFAILED\x1b[0m \x1b[90m(%s)\x1b[0m\n", (void *) old_ptr, size, location);
		exit (EXIT_FAILURE);
	}
	fprintf (stderr, "[e_debug] \x1b[36mrealloc\x1b[0m %p to %zu bytes: \x1b[32m%p\x1b[0m \x1b[90m(%s)\x1b[0m\n", (void *) old_ptr, size, ptr, location);

	return ptr;
}

void
(e_debug_free) (void *ptr, const char *location)
{
	fprintf (stderr, "[e_debug] \x1b[35mfree\x1b[0m \x1b[33m%p\x1b[0m \x1b[90m(%s)\x1b[0m\n", ptr, location);
	E_CONFIG_FREE_FUNC (ptr);
}

#endif /* E_DEBUG_IMPL */

#endif /* _EMPOWER_DEBUG_H_ */
