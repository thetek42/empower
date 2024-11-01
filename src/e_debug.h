#ifndef _EMPOWER_DEBUG_H_
#define _EMPOWER_DEBUG_H_

/*! e_debug *******************************************************************
 * 
 * This module provides utilities for quick debugging.
 *
 * Module dependencies:
 *  - e_log (optional)
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

#endif /* C_STDC_VERSION >= C_STDC_VERSION_C11 */

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

void e_debug_hexdump (const void *ptr, size_t len);

/******************************************************************************/

#ifdef E_DEBUG_IMPL

#include <string.h>

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

	if (p == nullptr) {
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

#endif /* E_DEBUG_IMPL */

#endif /* _EMPOWER_DEBUG_H_ */
