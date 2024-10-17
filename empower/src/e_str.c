#include "e_str.h"

#if E_CONFIG_MODULE_STR

#include <stdarg.h>

E_VEC_IMPLEMENT (char, __E_Vec_Char, __e_vec_char);

/* dynamic string functions ***************************************************/

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

	str = __e_vec_char_init_with_cap (1);
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
e_str_init_with_cap (usize cap)
{
	E_Str str;

	str = __e_vec_char_init_with_cap (cap > 0 ? cap : 1);
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
	return e_str_from_slice (s, strlen (s));
}

/**
 * Initialize a string from a slice of memory \s of length \len. The contents of
 * \s are copied into the string's buffer.
 */
E_Str
e_str_from_slice (const char *s, usize len)
{
	char *out;

	out = e_alloc (char, len + 1);
	memcpy (out, s, sizeof (char) * len);
	out[len] = 0;

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
e_str_from_allocated (char *ptr, usize len, usize cap)
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
	__e_vec_char_deinit (str);
}

/**
 * Grow the memory of a dynamically sized string \str to at least \cap items
 * (including the terminating nul character). If \cap is lower than the current
 * capacity of the string, no action will be performed. Internally, it rounds up
 * \cap to the nearest power of two and reallocates the memory to the new
 * capacity.
 */
void
e_str_grow (E_Str *str, usize cap)
{
	__e_vec_char_grow (str, cap);
}

/**
 * Clone a string. This function allocates memory which must be freed by the
 * user using \e_str_free. If \str is `nullptr`, an empty string is returned.
 */
E_Str
e_str_clone (E_Str *str)
{
	return __e_vec_char_clone (str);
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
usize
e_str_append_cstr (E_Str *str, const char *s)
{
	usize len;

	len = strlen (s);
	e_str_append_slice (str, s, len);

	return len;
}

/**
 * Append a string \s of length \len to the string \str and reallocate if
 * necessary.
 */
void
e_str_append_slice (E_Str *str, const char *s, usize len)
{
	if (!str) return;

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
C_ATTR_FORMAT (printf, 3, 4)
usize
e_str_append_fmt (E_Str *str, usize max_fmt_len, const char *fmt, ...)
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

	str->len += (usize) written < max_fmt_len
		? (usize) written
		: max_fmt_len;

	return (usize) written;
}


/**
 * Append a formatted string to the string \str. The required space is
 * determined automatically and the string is resized accordingly. tReturns he
 * number of bytes that were written to the string.
 */
C_ATTR_FORMAT (printf, 2, 3)
usize
e_str_append_fmt_all (E_Str *str, const char *fmt, ...)
{
	int written;
	va_list ap;

	if (!str) return 0;

	va_start (ap, fmt);
	written = vsnprintf (nullptr, 0, fmt, ap);
	va_end (ap);

	if (str->len + (usize) written + 1 > str->cap) {
		e_str_grow (str, str->len + (usize) written + 1);
	}

	va_start (ap, fmt);
	written = vsnprintf (&str->ptr[str->len], (usize) written + 1, fmt, ap);
	va_end (ap);

	str->len += (usize) written;

	return (usize) written;
}

/**
 * Insert a single character \c at position \idx into the string \str. If \idx
 * is out of bounds, the character is appended instead. When inserting into the
 * middle of the string, the rest of the string is pushed back. The memory is
 * reallocated if necessary.
 */
void
e_str_insert_char (E_Str *str, usize idx, char c)
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
usize
e_str_insert_cstr (E_Str *str, usize idx, const char *s)
{
	usize len;

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
e_str_insert_slice (E_Str *str, usize idx, const char *s, usize len)
{
	if (!str || !s || len == 0) return;
	if (idx >= str->len) {
		e_str_append_slice (str, s, len);
		return;
	}

	if (str->len + len + 1 > str->cap) {
		e_str_grow (str, str->len + len + 1);
	}

	memmove (&str->ptr[idx + len], &str->ptr[idx], str->len - idx + 1);
	strncpy (&str->ptr[idx], s, len);
	str->len += len;
}

/**
 * Remove \count characters from the string \str, starting at \idx. Returns the
 * number of characters removed. When removing from the middle of the stirng,
 * the memory inside the string is shifted accordingly.
 */
usize
e_str_remove (E_Str *str, usize idx, usize count)
{
	if (!str || count == 0 || idx >= str->len) return 0;
	if (idx + count == str->len) {
		str->len -= count;
		str->ptr[str->len] = 0;
		return count;
	}
	count = idx + count > str->len ? str->len - idx : count;
	memmove (&str->ptr[idx], &str->ptr[idx + count],
	         str->len - idx - count + 1);
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
	usize new_len;

	if (!str || !str->ptr) return;
	start = e_cstr_trim_start (str->ptr);
	new_len = str->len - (usize) (start - str->ptr);
	if (start != str->ptr) {
		memmove (str->ptr, start, new_len);
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
	usize new_len;

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

/* e_cstr wrappers ************************************************************/

/**
 * The `E_Str` wrapper for `e_str_count_char`.
 */
usize
e_str_count_char (const E_Str *s, char c)
{
	if (!s) return 0;
	return e_cstr_count_char (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_not`.
 */
usize
e_str_count_char_not (const E_Str *s, char c)
{
	if (!s) return 0;
	return e_cstr_count_char_not (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_pat`.
 */
usize
e_str_count_char_pat (const E_Str *s, const char *accept)
{
	if (!s) return 0;
	return e_cstr_count_char_pat (s->ptr, accept);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_not_pat`.
 */
usize
e_str_count_char_not_pat (const E_Str *s, const char *reject)
{
	if (!s) return 0;
	return e_cstr_count_char_not_pat (s->ptr, reject);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_func`.
 */
usize
e_str_count_char_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return 0;
	return e_cstr_count_char_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_str_count_char_not_func`.
 */
usize
e_str_count_char_not_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return 0;
	return e_cstr_count_char_not_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_str_count_str`.
 */
usize
e_str_count_str (const E_Str *haystack, const char *needle)
{
	if (!haystack) return 0;
	return e_cstr_count_str (haystack->ptr, needle);
}

/**
 * The `E_Str` wrapper for `e_str_count_str_overlap`.
 */
usize
e_str_count_str_overlap (const E_Str *haystack, const char *needle)
{
	if (!haystack) return 0;
	return e_cstr_count_str_overlap (haystack->ptr, needle);
}

/**
 * The `E_Str` wrapper for `e_str_count_lines`.
 */
usize
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
	if (!s) return nullptr;
	return e_cstr_find_char (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_not`.
 */
const char *
e_str_find_char_not (const E_Str *s, char c)
{
	if (!s) return nullptr;
	return e_cstr_find_char_not (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_pat`.
 */
const char *
e_str_find_char_pat (const E_Str *s, const char *accept)
{
	if (!s) return nullptr;
	return e_cstr_find_char_pat (s->ptr, accept);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_not_pat`.
 */
const char *
e_str_find_char_not_pat (const E_Str *s, const char *reject)
{
	if (!s) return nullptr;
	return e_cstr_find_char_not_pat (s->ptr, reject);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_func`.
 */
const char *
e_str_find_char_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return nullptr;
	return e_cstr_find_char_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_char_not_func`.
 */
const char *
e_str_find_char_not_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return nullptr;
	return e_cstr_find_char_not_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_find_str`.
 */
const char *
e_str_find_str (const E_Str *haystack, const char *needle)
{
	if (!haystack) return nullptr;
	return e_cstr_find_str (haystack->ptr, needle);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char`.
 */
const char *
e_str_rfind_char (const E_Str *s, char c)
{
	if (!s) return nullptr;
	return e_cstr_rfind_char (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_not`.
 */
const char *
e_str_rfind_char_not (const E_Str *s, char c)
{
	if (!s) return nullptr;
	return e_cstr_rfind_char_not (s->ptr, c);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_pat`.
 */
const char *
e_str_rfind_char_pat (const E_Str *s, const char *accept)
{
	if (!s) return nullptr;
	return e_cstr_rfind_char_pat (s->ptr, accept);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_not_pat`.
 */
const char *
e_str_rfind_char_not_pat (const E_Str *s, const char *reject)
{
	if (!s) return nullptr;
	return e_cstr_rfind_char_not_pat (s->ptr, reject);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_func`.
 */
const char *
e_str_rfind_char_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return nullptr;
	return e_cstr_rfind_char_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_char_not_func`.
 */
const char *
e_str_rfind_char_not_func (const E_Str *s, E_Char_Predicate func)
{
	if (!s) return nullptr;
	return e_cstr_rfind_char_not_func (s->ptr, func);
}

/**
 * The `E_Str` wrapper for `e_cstr_rfind_str`.
 */
const char *
e_str_rfind_str (const E_Str *haystack, const char *needle)
{
	if (!haystack) return nullptr;
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
e_str_eq_n (const E_Str *a, const char *b, usize n)
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
e_str_continues_with (const E_Str *s, const char *expect, usize pos)
{
	if (!s) return false;
	return e_cstr_continues_with (s->ptr, expect, pos);
}

/**
 * The `E_Str` wrapper for `e_str_distance`.
 */
usize
e_str_distance (const E_Str *a, const char *b)
{
	if (!a) return e_cstr_len (b);
	return e_cstr_distance (a->ptr, b);
}

#endif /* E_CONFIG_MODULE_STR */
