#ifndef _EMPOWER_STR_H_
#define _EMPOWER_STR_H_

/*! e_str *********************************************************************
 * 
 * This module provides a dynamically sized string type alongside various string
 * manipulation functions.
 *
 * Module dependencies:
 *  - e_cstr
 *  - e_vec
 *
 * Example:
 * | #define E_CSTR_IMPL
 * | #define E_STR_IMPL
 * | #include <e_str.h>
 * |
 * | E_Str s = e_str_init ();
 * | e_str_append_fmt (&s, "%s!", name);
 * | e_str_insert_cstr (&s, 0, "Hello, ");
 * | e_str_deinit (&s);
 *
 ******************************************************************************/

#include <e_cstr.h>
#include <e_vec.h>
#include <stdbool.h>
#include <stddef.h>

/* public interface ***********************************************************/

E_VEC_DECL (char, E_Vec_Char, e_vec_char);

/**
 * A dynamically sized string type, internally represented as a `E_Vec` of type
 * `char`. The string data can be accessed through the field \ptr, and is
 * always terminated with a nul character. Length and capacity are stored in the
 * fields \len and \cap, respectively.
 */
typedef E_Vec_Char E_Str;

/* --- dynamic string functions --- */

E_Str e_str_init (void);
E_Str e_str_init_with_cap (size_t cap);
E_Str e_str_from_cstr (const char *s);
E_Str e_str_from_slice (const char *s, size_t len);
E_Str e_str_from_allocated (char *ptr, size_t len, size_t cap);
void e_str_deinit (E_Str *str);
void e_str_grow (E_Str *str, size_t cap);
E_Str e_str_clone (E_Str *str);
void e_str_append_char (E_Str *str, char c);
size_t e_str_append_cstr (E_Str *str, const char *s);
void e_str_append_slice (E_Str *str, const char *s, size_t len);
__attribute__ ((format (printf, 3, 4))) size_t e_str_append_fmt_n (E_Str *str, size_t max_fmt_len, const char *fmt, ...);
__attribute__ ((format (printf, 2, 3))) size_t e_str_append_fmt (E_Str *str, const char *fmt, ...);
void e_str_insert_char (E_Str *str, size_t idx, char c);
size_t e_str_insert_cstr (E_Str *str, size_t idx, const char *s);
void e_str_insert_slice (E_Str *str, size_t idx, const char *s, size_t len);
size_t e_str_remove (E_Str *str, size_t idx, size_t count);
bool e_str_remove_prefix (E_Str *str, const char *prefix);
bool e_str_remove_suffix (E_Str *str, const char *suffix);
void e_str_trim_start (E_Str *str);
void e_str_trim_end (E_Str *str);
void e_str_trim (E_Str *str);

/* --- e_cstr wrappers --- */

size_t e_str_count_char (const E_Str *s, char c);
size_t e_str_count_char_not (const E_Str *s, char c);
size_t e_str_count_char_pat (const E_Str *s, const char *accept);
size_t e_str_count_char_not_pat (const E_Str *s, const char *reject);
size_t e_str_count_char_func (const E_Str *s, E_Char_Predicate func);
size_t e_str_count_char_not_func (const E_Str *s, E_Char_Predicate func);
size_t e_str_count_str (const E_Str *haystack, const char *needle);
size_t e_str_count_str_overlap (const E_Str *haystack, const char *needle);
size_t e_str_count_lines (const E_Str *s);
bool e_str_is_ascii (const E_Str *s);
bool e_str_is_blank (const E_Str *s);
bool e_str_matches_predicate (const E_Str *s, E_Char_Predicate func);
void e_str_to_ascii_lower (E_Str *s);
void e_str_to_ascii_upper (E_Str *s);
const char *e_str_find_char (const E_Str *s, char c);
const char *e_str_find_char_not (const E_Str *s, char c);
const char *e_str_find_char_pat (const E_Str *s, const char *accept);
const char *e_str_find_char_not_pat (const E_Str *s, const char *reject);
const char *e_str_find_char_func (const E_Str *s, E_Char_Predicate func);
const char *e_str_find_char_not_func (const E_Str *s, E_Char_Predicate func);
const char *e_str_find_str (const E_Str *haystack, const char *needle);
const char *e_str_rfind_char (const E_Str *s, char c);
const char *e_str_rfind_char_not (const E_Str *s, char c);
const char *e_str_rfind_char_pat (const E_Str *s, const char *accept);
const char *e_str_rfind_char_not_pat (const E_Str *s, const char *reject);
const char *e_str_rfind_char_func (const E_Str *s, E_Char_Predicate func);
const char *e_str_rfind_char_not_func (const E_Str *s, E_Char_Predicate func);
const char *e_str_rfind_str (const E_Str *haystack, const char *needle);
bool e_str_contains (const E_Str *haystack, const char *needle);
bool e_str_eq (const E_Str *a, const char *b);
bool e_str_eq_n (const E_Str *a, const char *b, size_t n);
bool e_str_starts_with (const E_Str *s, const char *expect);
bool e_str_ends_with (const E_Str *s, const char *expect);
bool e_str_continues_with (const E_Str *s, const char *expect, size_t pos);
size_t e_str_distance (const E_Str *a, const char *b);

/* implementation *************************************************************/

#ifdef E_STR_IMPL

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

E_VEC_IMPL (char, E_Vec_Char, e_vec_char);

/* --- dynamic string functions --- */

/**
 * Initialise a dynamically sized string with capacity 0. This function
 * allocates enough memory to store a string with a nul terminating byte. When
 * the allocation fails, it will terminate the programme with an error message.
 * The user must free the allocated memory with `e_str_free`.
 */
E_Str
e_str_init (void)
{
	E_Str str;

	str = e_vec_char_init_with_cap (1);
	str.ptr[0] = 0;

	return str;
}

/**
 * Initialise a dynamically sized string with a given capacity. This function
 * allocates enough memory to store \cap characters (including the terminating
 * nul character). When the allocation fails, it will terminate the programme
 * with an error message. The user must free the allocated memory with
 * `e_str_free`.
 */
E_Str
e_str_init_with_cap (size_t cap)
{
	E_Str str;

	str = e_vec_char_init_with_cap (cap > 0 ? cap : 1);
	str.ptr[0] = 0;

	return str;
}

/**
 * Initialize a string from a nul-terminated string \s of length \len. The
 * contents of \s are copied into the string's buffer.
 */
E_Str
e_str_from_cstr (const char *s)
{
	if (!s) return e_str_init ();
	return e_str_from_slice (s, strlen (s));
}

/**
 * Initialize a string from a slice of memory \s of length \len. The contents of
 * \s are copied into the string's buffer.
 */
E_Str
e_str_from_slice (const char *s, size_t len)
{
	char *out;

	if (!s || len == 0) return e_str_init ();
	out = malloc (sizeof (char) * len);
	if (!out) {
		fprintf (stderr, "[e_str] failed to alloc %zu bytes\n", len);
		exit (EXIT_FAILURE);
	}
	memcpy (out, s, sizeof (char) * len);

	return (E_Str) {
		.ptr = out,
		.len = len,
		.cap = len,
	};
}

/**
 * Initialize a string from a pre-allocated nul-terminated string \ptr. The
 * memory owned by \ptr should be capable of holding at least \cap bytes. \len
 * is the length of \ptr. The returned string takes ownership of \ptr.
 */
E_Str
e_str_from_allocated (char *ptr, size_t len, size_t cap)
{
	return (E_Str) {
		.ptr = ptr,
		.len = len,
		.cap = cap,
	};
}

/**
 * Deinitialise a dynamically sized string and free the memory occupied by it.
 */
void
e_str_deinit (E_Str *str)
{
	e_vec_char_deinit (str);
}

/**
 * Grow the memory of a dynamically sized string \str to at least \cap items
 * (including the terminating nul character). If \cap is lower than the current
 * capacity of the string, no action will be performed. Internally, it rounds up
 * \cap to the nearest power of two and reallocates the memory to the new
 * capacity.
 */
void
e_str_grow (E_Str *str, size_t cap)
{
	e_vec_char_grow (str, cap);
}

/**
 * Clone a string. This function allocates memory which must be freed by the
 * user using \e_str_free. If \str is `nullptr`, an empty string is returned.
 */
E_Str
e_str_clone (E_Str *str)
{
	return e_vec_char_clone (str);
}

/**
 * Append a single character \c to the string \str and reallocate if necessary.
 */
void
e_str_append_char (E_Str *str, char c)
{
	e_str_append_slice (str, &c, 1);
}

/**
 * Append a nul-terminated string \s to the string \str and reallocate if
 * necessary. Returns `strlen (s)`.
 */
size_t
e_str_append_cstr (E_Str *str, const char *s)
{
	size_t len;

	if (!s) return 0;
	len = strlen (s);
	e_str_append_slice (str, s, len);

	return len;
}

/**
 * Append a string \s of length \len to the string \str and reallocate if
 * necessary.
 */
void
e_str_append_slice (E_Str *str, const char *s, size_t len)
{
	if (!str) return;

	if (!s || len == 0) return;
	if (str->len + len + 1 > str->cap) {
		e_str_grow (str, str->len + len + 1);
	}

	strncpy (&str->ptr[str->len], s, len);
	str->ptr[str->len + len] = 0;
	str->len += len;
}

/**
 * Append a formatted string to the string \str and reallocate if necessary.
 * This function works using `vsnprintf`: \fmt serves as the formatting
 * specifier, and only up to \max_fmt_len bytes are actually written to the
 * string. Returns the number of bytes that would have been written if the
 * entire formatted string could have been appended. If the entire string was
 * appended, this is equal to the number of bytes actually written.
 */
__attribute__ ((format (printf, 3, 4)))
size_t
e_str_append_fmt_n (E_Str *str, size_t max_fmt_len, const char *fmt, ...)
{
	int written;
	va_list ap;

	if (!str) return 0;

	if (str->len + max_fmt_len + 1 > str->cap) {
		e_str_grow (str, str->len + max_fmt_len + 1);
	}

	va_start (ap, fmt);
	written = vsnprintf (&str->ptr[str->len], max_fmt_len + 1, fmt, ap);
	va_end (ap);

	str->len += (size_t) written < max_fmt_len
	            ? (size_t) written
	            : max_fmt_len;

	return (size_t) written;
}


/**
 * Append a formatted string to the string \str. The required space is
 * determined automatically and the string is resized accordingly. tReturns he
 * number of bytes that were written to the string.
 */
__attribute__ ((format (printf, 2, 3)))
size_t
e_str_append_fmt (E_Str *str, const char *fmt, ...)
{
	int written;
	va_list ap;

	if (!str) return 0;

	va_start (ap, fmt);
	written = vsnprintf (NULL, 0, fmt, ap);
	va_end (ap);

	if (str->len + (size_t) written + 1 > str->cap) {
		e_str_grow (str, str->len + (size_t) written + 1);
	}

	va_start (ap, fmt);
	written = vsnprintf (&str->ptr[str->len], (size_t) written + 1, fmt, ap);
	va_end (ap);

	str->len += (size_t) written;

	return (size_t) written;
}

/**
 * Insert a single character \c at position \idx into the string \str. If \idx
 * is out of bounds, the character is appended instead. When inserting into the
 * middle of the string, the rest of the string is pushed back. The memory is
 * reallocated if necessary.
 */
void
e_str_insert_char (E_Str *str, size_t idx, char c)
{
	e_str_insert_slice (str, idx, &c, 1);
}

/**
 * Insert a nul-terminated string \s at position \idx into the string \str. If
 * \idx is out of bounds, the character is appended instead. When inserting into
 * the middle of the string, the rest of the string is pushed back. The memory
 * is reallocated if necessary. Retruns the number of characters added to \str,
 * i.e. the length of \s.
 */
size_t
e_str_insert_cstr (E_Str *str, size_t idx, const char *s)
{
	size_t len;

	if (!s) return 0;
	len = strlen (s);
	e_str_insert_slice (str, idx, s, len);

	return len;
}

/**
 * Insert a string \s of length \len at position \idx into the string \str. If
 * \idx is out of bounds, the character is appended instead. When inserting into
 * the middle of the string, the rest of the string is pushed back. The memory
 * is reallocated if necessary.
 */
void
e_str_insert_slice (E_Str *str, size_t idx, const char *s, size_t len)
{
	if (!str || !s || len == 0) return;
	if (idx >= str->len) {
		e_str_append_slice (str, s, len);
		return;
	}

	if (str->len + len + 1 > str->cap) {
		e_str_grow (str, str->len + len + 1);
	}

	memmove (&str->ptr[idx + len], &str->ptr[idx],
	         sizeof (char) * (str->len - idx + 1));
	strncpy (&str->ptr[idx], s, len);
	str->len += len;
}

/**
 * Remove \count characters from the string \str, starting at \idx. Returns the
 * number of characters removed. When removing from the middle of the stirng,
 * the memory inside the string is shifted accordingly.
 */
size_t
e_str_remove (E_Str *str, size_t idx, size_t count)
{
	if (!str || count == 0 || idx >= str->len) return 0;
	if (idx + count == str->len) {
		str->len -= count;
		str->ptr[str->len] = 0;
		return count;
	}
	count = idx + count > str->len ? str->len - idx : count;
	memmove (&str->ptr[idx], &str->ptr[idx + count],
	         sizeof (char) * (str->len - idx - count + 1));
	str->len -= count;
	str->ptr[str->len] = 0;
	return count;
}

/**
 * Remove a prefix from a string \str. If the prefix as given by the
 * nul-terminated string \prefix is found, it is removed. Otherwise, no action
 * is performed. Returns whether the prefix was removed, or `false` if one of
 * the parameters was `nullptr`.
 */
bool
e_str_remove_prefix (E_Str *str, const char *prefix)
{
	if (!str || !prefix) return false;
	if (e_cstr_starts_with (str->ptr, prefix)) {
		e_str_remove (str, 0, strlen (prefix));
		return true;
	}
	return false;
}

/**
 * Remove a suffix from a string \str. If the suffix as given by the
 * nul-terminated string \suffix is found, it is removed. Otherwise, no action
 * is performed. Returns whether the suffix was removed, or `false` if one of
 * the parameters was `nullptr`.
 */
bool
e_str_remove_suffix (E_Str *str, const char *suffix)
{
	if (!str || !suffix) return false;
	if (e_cstr_ends_with (str->ptr, suffix)) {
		str->len -= strlen (suffix);
		str->ptr[str->len] = 0;
		return true;
	}
	return false;
}

/**
 * Trim whitespace at the start of the string \str. \str is modified.
 */
void
e_str_trim_start (E_Str *str)
{
	const char *start;
	size_t new_len;

	if (!str || !str->ptr) return;
	start = e_cstr_trim_start (str->ptr);
	new_len = str->len - (size_t) (start - str->ptr);
	if (start != str->ptr) {
		memmove (str->ptr, start, sizeof (char) * new_len);
		str->ptr[new_len] = 0;
		str->len = new_len;
	}
}

/**
 * Trim whitespace at the end of the string \str. \str is modified.
 */
void
e_str_trim_end (E_Str *str)
{
	size_t new_len;

	if (!str || !str->ptr) return;
	new_len = e_cstr_trim_end_with_len (str->ptr, str->len);
	str->len = new_len;
	str->ptr[str->len] = 0;
}

/**
 * Trim whitespace at the start and end of the string \str. \str is modified.
 */
void
e_str_trim (E_Str *str)
{
	if (!str) return;
	e_str_trim_end (str);
	e_str_trim_start (str);
}

/* --- e_cstr wrappers --- */

/**
 * The `E_Str` wrapper for `e_str_count_char`.
 */
size_t
e_str_count_char (const E_Str *s, char c)
{
	if (!s) return 0;
	return e_cstr_count_char (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_not`.
 */
size_t
e_str_count_char_not (const E_Str *s, char c)
{
	if (!s) return 0;
	return e_cstr_count_char_not (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_pat`.
 */
size_t
e_str_count_char_pat (const E_Str *s, const char *accept)
{
	if (!s) return 0;
	return e_cstr_count_char_pat (s->ptr, accept);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_not_pat`.
 */
size_t
e_str_count_char_not_pat (const E_Str *s, const char *reject)
{
	if (!s) return 0;
	return e_cstr_count_char_not_pat (s->ptr, reject);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_func`.
 */
size_t
e_str_count_char_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return 0;
	return e_cstr_count_char_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_not_func`.
 */
size_t
e_str_count_char_not_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return 0;
	return e_cstr_count_char_not_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_str_count_str`.
 */
size_t
e_str_count_str (const E_Str *haystack, const char *needle)
{
	if (!haystack) return 0;
	return e_cstr_count_str (haystack->ptr, needle);
}

/**
 * The `E_Str` wrapper for `e_str_count_str_overlap`.
 */
size_t
e_str_count_str_overlap (const E_Str *haystack, const char *needle)
{
	if (!haystack) return 0;
	return e_cstr_count_str_overlap (haystack->ptr, needle);
}

/**
 * The `E_Str` wrapper for `e_str_count_lines`.
 */
size_t
e_str_count_lines (const E_Str *s)
{
	if (!s) return 0;
	return e_cstr_count_lines (s->ptr);
}

/**
 * The `E_Str` wrapper for `e_cstr_is_ascii`.
 */
bool
e_str_is_ascii (const E_Str *s)
{
	if (!s) return 0;
	return e_cstr_is_ascii (s->ptr);
}

/**
 * The `E_Str` wrapper for `e_cstr_is_blank`.
 */
bool
e_str_is_blank (const E_Str *s)
{
	if (!s) return 0;
	return e_cstr_is_blank (s->ptr);
}

/**
 * The `E_Str` wrapper for `e_cstr_matches_predicate`.
 */
bool
e_str_matches_predicate (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return 0;
	return e_cstr_matches_predicate (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_to_ascii_lower`.
 */
void
e_str_to_ascii_lower (E_Str *s)
{
	if (s) e_cstr_to_ascii_lower (s->ptr);
}

/**
 * The `E_Str` wrapper for `e_cstr_to_ascii_upper`.
 */
void
e_str_to_ascii_upper (E_Str *s)
{
	if (s) e_cstr_to_ascii_upper (s->ptr);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char`.
 */
const char *
e_str_find_char (const E_Str *s, char c)
{
	if (!s) return NULL;
	return e_cstr_find_char (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_not`.
 */
const char *
e_str_find_char_not (const E_Str *s, char c)
{
	if (!s) return NULL;
	return e_cstr_find_char_not (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_pat`.
 */
const char *
e_str_find_char_pat (const E_Str *s, const char *accept)
{
	if (!s) return NULL;
	return e_cstr_find_char_pat (s->ptr, accept);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_not_pat`.
 */
const char *
e_str_find_char_not_pat (const E_Str *s, const char *reject)
{
	if (!s) return NULL;
	return e_cstr_find_char_not_pat (s->ptr, reject);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_func`.
 */
const char *
e_str_find_char_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return NULL;
	return e_cstr_find_char_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_not_func`.
 */
const char *
e_str_find_char_not_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return NULL;
	return e_cstr_find_char_not_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_str`.
 */
const char *
e_str_find_str (const E_Str *haystack, const char *needle)
{
	if (!haystack) return NULL;
	return e_cstr_find_str (haystack->ptr, needle);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char`.
 */
const char *
e_str_rfind_char (const E_Str *s, char c)
{
	if (!s) return NULL;
	return e_cstr_rfind_char (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_not`.
 */
const char *
e_str_rfind_char_not (const E_Str *s, char c)
{
	if (!s) return NULL;
	return e_cstr_rfind_char_not (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_pat`.
 */
const char *
e_str_rfind_char_pat (const E_Str *s, const char *accept)
{
	if (!s) return NULL;
	return e_cstr_rfind_char_pat (s->ptr, accept);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_not_pat`.
 */
const char *
e_str_rfind_char_not_pat (const E_Str *s, const char *reject)
{
	if (!s) return NULL;
	return e_cstr_rfind_char_not_pat (s->ptr, reject);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_func`.
 */
const char *
e_str_rfind_char_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return NULL;
	return e_cstr_rfind_char_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_not_func`.
 */
const char *
e_str_rfind_char_not_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return NULL;
	return e_cstr_rfind_char_not_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_str`.
 */
const char *
e_str_rfind_str (const E_Str *haystack, const char *needle)
{
	if (!haystack) return NULL;
	return e_cstr_rfind_str (haystack->ptr, needle);
}

/**
 * The `E_Str` wrapper for `e_str_contains`.
 */
bool
e_str_contains (const E_Str *haystack, const char *needle)
{
	if (!haystack) return false;
	return e_cstr_contains (haystack->ptr, needle);
}

/**
 * The `E_Str` wrapper for `e_str_eq`.
 */
bool
e_str_eq (const E_Str *a, const char *b)
{
	if (!a && !b) return true;
	if (!a) return false;
	return e_cstr_eq (a->ptr, b);
}

/**
 * The `E_Str` wrapper for `e_str_eq_n`.
 */
bool
e_str_eq_n (const E_Str *a, const char *b, size_t n)
{
	if (!a && !b) return true;
	if (!a) return false;
	return e_cstr_eq_n (a->ptr, b, n);
}

/**
 * The `E_Str` wrapper for `e_str_starts_with`.
 */
bool
e_str_starts_with (const E_Str *s, const char *expect)
{
	if (!s) return false;
	return e_cstr_starts_with (s->ptr, expect);
}

/**
 * The `E_Str` wrapper for `e_str_ends_with`.
 */
bool
e_str_ends_with (const E_Str *s, const char *expect)
{
	if (!s) return false;
	return e_cstr_ends_with (s->ptr, expect);
}

/**
 * The `E_Str` wrapper for `e_str_continues_with`.
 */
bool
e_str_continues_with (const E_Str *s, const char *expect, size_t pos)
{
	if (!s) return false;
	return e_cstr_continues_with (s->ptr, expect, pos);
}

/**
 * The `E_Str` wrapper for `e_str_distance`.
 */
size_t
e_str_distance (const E_Str *a, const char *b)
{
	if (!a) return e_cstr_len (b);
	return e_cstr_distance (a->ptr, b);
}

#endif /* E_STR_IMPL */

#endif /* _EMPOWER_STR_H_ */
