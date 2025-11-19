#ifndef EMPOWER_DEBUG_H_
#define EMPOWER_DEBUG_H_

/******************************************************************************
 *
 * This module provides utilities for debugging.
 *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef E_STRINGIFY
# define E_STRINGIFY_HELPER_(x) #x
# define E_STRINGIFY(x) E_STRINGIFY_HELPER_ (x)
#endif
#ifndef E_SOURCE_LOCATION
# define E_SOURCE_LOCATION __FILE__ ":" E_STRINGIFY (__LINE__)
#endif

#if __STDC_VERSION__ >= 201112L

/**
 * Obtain the formatting argument for printf-like functions for the generic
 * argument `value`. Works for regular integers and floats. All other types are
 * treated as pointers.
 */
# ifndef E_AUTO_FMT
#  define E_AUTO_FMT(value) _Generic ((value), \
        char: "%c", \
        signed char: "%d", \
        signed short: "%d", \
        signed int: "%d", \
        signed long: "%ld", \
        signed long long: "%lld", \
        unsigned char: "%u", \
        unsigned short: "%u", \
        unsigned int: "%u", \
        unsigned long: "%lu", \
        unsigned long long: "%llu", \
        float: "%f", \
        double: "%g", \
        long double: "%lg", \
        _Bool: "%d", \
        default: "%p")
# endif

/**
 * Debug-print the generic argument `value`. For a call like `e_debug (x + y)`,
 * it will print `x + y = 42` with the regular debug format from e_log, where 42
 * is the result of the addition. Works with every type that is accepted by
 * `E_DEBUG_AUTO_FMT`.
 */
# define e_debug(value) \
	do { \
		fprintf (stderr, "[debug] " E_STRINGIFY (value) " = "); \
		fprintf (stderr, E_AUTO_FMT(value), (value)); \
		fprintf (stderr, " (" E_SOURCE_LOCATION ")\n"); \
	} while (0)

#endif /* __STDC_VERSION__ >= 201112L */

/**
 * This is the same as `assert()`, but with slightly better error reporting.
 */
#ifdef NDEBUG
# define e_assert(expr, msg)
#else /* NDEBUG */
# define e_assert(expr, msg) \
	do { \
		if (!(expr)) { \
			fprintf (stderr, \
                                 "Assertion \"%s\" (at " E_SOURCE_LOCATION \
                                 ") failed: " msg "\n", #expr); \
		} \
	} while (0)
#endif /* NDEBUG */

#define e_debug_alloc(type, nmemb) ((e_debug_alloc_size) (sizeof (type) * (nmemb), E_SOURCE_LOCATION))
#define e_debug_alloc_size(size) ((e_debug_alloc_size) ((size), E_SOURCE_LOCATION))
#define e_debug_alloc_zero(type, nmemb) ((e_debug_alloc_zero_size) (sizeof (type) * (nmemb), E_SOURCE_LOCATION))
#define e_debug_alloc_zero_size(size) ((e_debug_alloc_zero_size) ((size), E_SOURCE_LOCATION))
#define e_debug_realloc(ptr, type, nmemb) ((e_debug_realloc_size) ((ptr), sizeof (type) * (nmemb), E_SOURCE_LOCATION))
#define e_debug_realloc_size(ptr, size) ((e_debug_realloc_size) ((ptr), (size), E_SOURCE_LOCATION))
#define e_debug_new(type) ((e_debug_alloc_size) (sizeof (type), E_SOURCE_LOCATION))
#define e_debug_new_zero(type) ((e_debug_alloc_zero_size) (sizeof (type), E_SOURCE_LOCATION))
#define e_debug_free(ptr) ((e_debug_free) ((ptr), E_SOURCE_LOCATION))

#if __STDC_VERSION__ >= 202311L && !defined (_MSC_VER)
# define e_unreachable() unreachable ()
#elif defined (__GNUC__) || defined (__clang__) || defined (__TINYC__)
# define e_unreachable() __builtin_unreachable ()
#elif defined (_MSC_VER)
# define e_unreachable() __assume (0)
#else
# include <assert.h>
# define e_unreachable() assert (0)
#endif

#define e_todo(msg) \
	do { \
		fprintf (stderr, "//TODO: " msg " (" E_SOURCE_LOCATION ")\n"); \
		exit (EXIT_FAILURE); \
	} while (0)

#define e_unimplemented(msg) \
	do { \
		fprintf (stderr, "not implemented: " msg " (" E_SOURCE_LOCATION ")\n"); \
		exit (EXIT_FAILURE); \
	} while (0)

void e_debug_hexdump (const void *ptr, size_t len);
void *(e_debug_alloc_size) (size_t size, const char *location);
void *(e_debug_alloc_zero_size) (size_t size, const char *location);
void *(e_debug_realloc_size) (void *ptr, size_t size, const char *location);
void (e_debug_free) (void *ptr, const char *location);

/******************************************************************************/

#ifdef E_DEBUG_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ >= 199901L
# define ERROR_MSG(size, location) "[e_debug] failed to allocate %zu bytes at %s\n", size, location
# define REALLOC_ERROR_MSG(oldptr, size, location) "[e_debug] failed to reallocate %s to %zu bytes at %s\n", oldptr, size, location
# define SUCCESS_MSG(ptr, size, location) "[e_debug] allocated %zu bytes at %s => %p\n", size, location, ptr
# define REALLOC_SUCCESS_MSG(oldptr, newptr, size, location) "[e_debug] reallocated %s to %zu bytes at %s => %p\n", oldptr, size, location, newptr
#else
# define ERROR_MSG(size, location) "[e_debug] failed to allocate at %s\n", location
# define REALLOC_ERROR_MSG(ptr, size, location) "[e_debug] failed to reallocate %p at %s\n", ptr, location
# define SUCCESS_MSG(ptr, size, location) "[e_debug] allocated at %s => %p\n", location, ptr
# define REALLOC_SUCCESS_MSG(ptr, newptr, size, location) "[e_debug] reallocated %p at %s => %p\n", ptr, location, newptr
#endif
#define FREE_MSG(ptr, location) "[e_debug] pointer %p freed at %s\n", ptr, location
#define CHECK_POINTER(ptr, size, location) do { \
                if ((ptr) == NULL) { \
                        fprintf (stderr, ERROR_MSG (size, location)); \
                        exit (EXIT_FAILURE); \
                } \
        } while (0)

/**
 * Print a hexdump of a region of memory.
 */
void
e_debug_hexdump (const void *ptr, size_t len)
{
	const unsigned char *p = ptr;
	size_t i, j, k, l;
	char buf[70] = {0};
	unsigned char c;

	fprintf (stderr, "\n---=== hexdump %p ===---\n\n", (void *) p);
	for (i = 0; i < len; i += 16) {
		memset (buf, ' ', 69);
		sprintf (buf, "%08lu", (unsigned long) i);
		buf[8] = ':';
		for (j = 0; j < 16; j += 1) {
			if (i + j >= len) break;
			c = p[i + j];
			k = 10 + (j * 2) + (j / 2);
			l = 51 + j + (j / 8);
			buf[k] = (char) ((c / 16 >= 10) ? ((c / 16) + 87) : ((c / 16) + 48));
			buf[k + 1] = (char) ((c % 16 >= 10) ? ((c % 16) + 87) : ((c % 16) + 48));
			buf[l] = (char) ((32 <= c && c <= 126) ? c : '.');
		}
		fprintf (stderr, "%s\n", buf);
	}
	fputc ('\n', stderr);
}

void *
(e_debug_alloc_size) (size_t size, const char *location)
{
	void *ptr;
	ptr = malloc (size);
        CHECK_POINTER (ptr, size, location);
	memset (ptr, 0xCD, size);
        fprintf (stderr, SUCCESS_MSG (ptr, size, location));
	return ptr;
}

void *
(e_debug_alloc_zero_size) (size_t size, const char *location)
{
	void *ptr;
	ptr = calloc (size, sizeof (unsigned char));
        CHECK_POINTER (ptr, size, location);
        fprintf (stderr, SUCCESS_MSG (ptr, size, location));
	return ptr;
}

void *
(e_debug_realloc_size) (void *ptr, size_t size, const char *location)
{
        char oldptr[17];
	void *newptr;

	if (size == 0) {
                fprintf (stderr, FREE_MSG (ptr, location));
		return NULL;
	}

        /* save pointer for later to prevent the compiler from complaining */
        sprintf (oldptr, "%p", ptr);
	newptr = realloc (ptr, size);
        if (newptr == NULL) {
                fprintf (stderr, REALLOC_ERROR_MSG (oldptr, size, location)); \
                exit (EXIT_FAILURE);
        }
        fprintf (stderr, REALLOC_SUCCESS_MSG (oldptr, newptr, size, location));

	return newptr;
}

void
(e_debug_free) (void *ptr, const char *location)
{
        fprintf (stderr, FREE_MSG (ptr, location));
	free (ptr);
}

#undef CHECK_POINTER
#undef FREE_MSG
#undef REALLOC_SUCCESS_MSG
#undef SUCCESS_MSG
#undef REALLOC_ERROR_MSG
#undef ERROR_MSG

#endif /* E_DEBUG_IMPL */

#endif /* EMPOWER_DEBUG_H_ */
