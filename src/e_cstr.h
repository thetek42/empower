#ifndef _EMPOWER_CSTR_H_
#define _EMPOWER_CSTR_H_

/*! e_cstr ********************************************************************
 *
 * This module provides various utility functions for dealing with C strings and
 * serves as an extension to the `string.h` header from the standard library.
 *
 * Configuration options:
 *  - `E_CONFIG_MALLOC_FUNC` (default `malloc`): The function to use for allocating memory.
 *  - `E_CONFIG_FREE_FUNC` (default `free`): The function to use for freeing memory.
 *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>

/* public interface ***********************************************************/

/**
 * A pointer to a function that evaulates whether the ASCII character \c matches
 * a certain predicate. It returns 0 if \c is accepted, and non-zero if it is
 * rejected. The functions from `<ctype.h>` have this type.
 */
typedef int (* E_Char_Predicate) (int c);

typedef struct {
	size_t num_items;
	size_t next_item;
	char *buf;
	char *next;
} E_Str_Split;

size_t e_cstr_count_char (const char *s, char c);
size_t e_cstr_count_char_not (const char *s, char c);
size_t e_cstr_count_char_pat (const char *s, const char *accept);
size_t e_cstr_count_char_not_pat (const char *s, const char *reject);
size_t e_cstr_count_char_func (const char *s, E_Char_Predicate func);
size_t e_cstr_count_char_not_func (const char *s, E_Char_Predicate func);
size_t e_cstr_count_str (const char *haystack, const char *needle);
size_t e_cstr_count_str_overlap (const char *haystack, const char *needle);
size_t e_cstr_count_lines (const char *s);
size_t e_cstr_len (const char *s);
bool e_cstr_is_ascii (const char *s);
bool e_cstr_is_blank (const char *s);
bool e_cstr_matches_predicate (const char *s, E_Char_Predicate func);
char *e_cstr_to_ascii_lower (char *s);
char *e_cstr_to_ascii_upper (char *s);
char *e_cstr_to_ascii_lower_buf (const char *restrict src, char *restrict dest);
char *e_cstr_to_ascii_upper_buf (const char *restrict src, char *restrict dest);
const char *e_cstr_find_char (const char *s, char c);
const char *e_cstr_find_char_not (const char *s, char c);
const char *e_cstr_find_char_pat (const char *s, const char *accept);
const char *e_cstr_find_char_not_pat (const char *s, const char *reject);
const char *e_cstr_find_char_func (const char *s, E_Char_Predicate func);
const char *e_cstr_find_char_not_func (const char *s, E_Char_Predicate func);
const char *e_cstr_find_str (const char *haystack, const char *needle);
const char *e_cstr_rfind_char (const char *s, char c);
const char *e_cstr_rfind_char_not (const char *s, char c);
const char *e_cstr_rfind_char_pat (const char *s, const char *accept);
const char *e_cstr_rfind_char_not_pat (const char *s, const char *reject);
const char *e_cstr_rfind_char_func (const char *s, E_Char_Predicate func);
const char *e_cstr_rfind_char_not_func (const char *s, E_Char_Predicate func);
const char *e_cstr_rfind_str (const char *haystack, const char *needle);
bool e_cstr_contains (const char *haystack, const char *needle);
bool e_cstr_eq (const char *a, const char *b);
bool e_cstr_eq_n (const char *a, const char *b, size_t n);
const char *e_cstr_trim_start (const char *s);
size_t e_cstr_trim_end (const char *s);
size_t e_cstr_trim_end_with_len (const char *s, size_t len);
const char *e_cstr_trim (const char *s, size_t *len);
const char *e_cstr_trim_with_len (const char *s, size_t *len);
bool e_cstr_starts_with (const char *s, const char *expect);
bool e_cstr_ends_with (const char *s, const char *expect);
bool e_cstr_continues_with (const char *s, const char *expect, size_t pos);
size_t e_cstr_distance (const char *a, const char *b);
E_Str_Split e_cstr_split_char (const char *s, char c);
E_Str_Split e_cstr_split_str (const char *s, const char *delim);
E_Str_Split e_cstr_split_lines (const char *s);
const char *e_cstr_split_next (E_Str_Split *split);
void e_cstr_split_deinit (E_Str_Split *split);
int e_char_is_ascii (int c);

/* implementation *************************************************************/

#ifdef E_CSTR_IMPL

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef E_CONFIG_MALLOC_FUNC
# define E_CONFIG_MALLOC_FUNC malloc
#endif /* E_CONFIG_MALLOC_FUNC */
#ifndef E_CONFIG_FREE_FUNC
# define E_CONFIG_FREE_FUNC free
#endif /* E_CONFIG_FREE_FUNC */

/**
 * Count the number of occurances of a character \c in the nul-terminated string
 * \s. When \s is `nullptr`, it returns 0.
 */
size_t
e_cstr_count_char (const char *s, char c)
{
	size_t r;

	r = 0;
	if (!s) return 0;
	while (*s) {
		if (*s++ == c) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters which are not \c in the
 * nul-terminated string \s. When \s is `nullptr`, it returns 0.
 */
size_t
e_cstr_count_char_not (const char *s, char c)
{
	size_t r;

	r = 0;
	if (!s) return 0;
	while (*s) {
		if (*s++ != c) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters in the nul-terminated string \s
 * which are contained in the nul-terminated string \accept. When \s or \accept
 * is `nullptr`, it returns 0.
 */
size_t
e_cstr_count_char_pat (const char *s, const char *accept)
{
	size_t r;

	r = 0;
	if (!s || !accept) return 0;
	while (*s) {
		if (strchr (accept, *s++)) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters in the nul-terminated string \s
 * which are not contained in the nul-terminated string \reject. When \s is
 * `nullptr`, it returns 0. When \reject is `nullptr`, it returns `strlen (s)`.
 */
size_t
e_cstr_count_char_not_pat (const char *s, const char *reject)
{
	size_t r;

	r = 0;
	if (!s) return 0;
	if (!reject) return strlen (s);
	while (*s) {
		if (!strchr (reject, *s++)) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters in the nul-terminated string \s
 * which are accepted by the `E_Char_Predicate` \func. When \s or \func are
 * `nullptr`, it returns 0.
 */
size_t
e_cstr_count_char_func (const char *s, E_Char_Predicate func)
{
	size_t r;

	r = 0;
	if (!s || !func) return 0;
	while (*s) {
		if (func (*s++)) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters in the nul-terminated string \s
 * which are rejected by the `E_Char_Predicate` \func. When \s or \func are
 * `nullptr`, it returns 0.
 */
size_t
e_cstr_count_char_not_func (const char *s, E_Char_Predicate func)
{
	size_t r;

	r = 0;
	if (!s || !func) return 0;
	while (*s) {
		if (!func (*s++)) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of a nul-terminated string \needle in the
 * nul-terminated string \haystack. When \haystack or \needle is `nullptr`, it
 * returns 0. Overlap is not counted, so searching for "aa" in the string "aaa"
 * would yield 1.
 */
size_t
e_cstr_count_str (const char *haystack, const char *needle)
{
	size_t haystack_len, needle_len, count;

	if (!haystack || !needle) return 0;

	haystack_len = strlen (haystack);
	needle_len = strlen (needle);
	if (needle_len == 0) return haystack_len;
	count = 0;

	while (needle_len <= haystack_len) {
		if (strncmp (haystack, needle, needle_len) == 0) {
			count += 1;
			haystack += needle_len;
			haystack_len -= needle_len;
		} else {
			haystack += 1;
			haystack_len -= 1;
		}
	}

	return count;
}

/**
 * Count the number of occurances of a nul-terminated string \needle in the
 * nul-terminated string \haystack. When \haystack or \needle is `nullptr`, it
 * returns 0. Overlap is counted, so searching for "aa" in the string "aaa"
 * would yield 2.
 */
size_t
e_cstr_count_str_overlap (const char *haystack, const char *needle)
{
	size_t haystack_len, needle_len, count;

	if (!haystack || !needle) return 0;

	haystack_len = strlen (haystack);
	needle_len = strlen (needle);
	if (needle_len == 0) return haystack_len;
	count = 0;

	while (needle_len <= haystack_len) {
		if (strncmp (haystack, needle, needle_len) == 0) {
			count += 1;
		}
		haystack += 1;
		haystack_len -= 1;
	}

	return count;
}

/**
 * Count the number of lines in the nul-terminated string \s. When \s is
 * `nullptr`, it returns 0.
 */
size_t
e_cstr_count_lines (const char *s)
{
	if (!s) return 0;
	return 1 + e_cstr_count_char (s, '\n');
}

/**
 * Get the length of a string \s. If \s is `nullptr`, 0 is returned. This is
 * essentially like `strlen`, except that it allows for `nullptr` (which would
 * cause undefined behaviour in `strlen`)
 */
size_t e_cstr_len (const char *s)
{
	return s ? strlen (s) : 0;
}

/**
 * Check if the nul-terminated string \s is ASCII. Returns `true` when \s only
 * consists of ASCII characters or if \s is `nullptr`.
 */
bool
e_cstr_is_ascii (const char *s)
{
	return e_cstr_matches_predicate (s, e_char_is_ascii);
}

/**
 * Check if the nul-terminated string \s only consists of whitespace characters
 * as per `isspace`. Returns `true` when \s only consists of whitespace or if \s
 * is `nullptr`.
 */
bool
e_cstr_is_blank (const char *s)
{
	return e_cstr_matches_predicate (s, isspace);
}

/**
 * Check if the nul-terminated string \s matches the `E_Char_Predicate` \func.
 * Returns `true` when \s only consists of characters that were accepted by
 * \func or if \s is `nullptr`, and returns false if \func is `nullptr` or did
 * not match.
 */
bool
e_cstr_matches_predicate (const char *s, E_Char_Predicate func)
{
	if (!s) return true;
	if (!func) return false;
	while (*s) {
		if (!func (*s++)) return false;
	}
	return true;
}

/**
 * Convert uppercase ASCII characters the nul-terminated string \s to lowercase
 * in-place. When \s is a `nullptr`, no action is performed. Returns \s.
 */
char *
e_cstr_to_ascii_lower (char *s)
{
	char *p;

	p = s;
	while (s && *p) {
		*p = (char) tolower (*p);
		p += 1;
	}
	return s;
}

/**
 * Convert lowercase ASCII characters the nul-terminated string \s to uppercase
 * in-place. When \s is a `nullptr`, no action is performed. Returns \s.
 */
char *
e_cstr_to_ascii_upper (char *s)
{
	char *p;

	p = s;
	while (s && *p) {
		*p = (char) toupper (*p);
		p += 1;
	}
	return s;
}

/**
 * Convert uppercase ASCII characters the nul-terminated string \src to
 * lowercase and store them in \dest. The terminating nul byte in \dest is
 * written. When \src or \dest are `nullptr`s, no action is performed. \dest
 * must have enough space to store all of \src, including the terminating nul
 * byte.
 */
char *
e_cstr_to_ascii_lower_buf (const char *restrict src, char *restrict dest)
{
	if (!src || !dest) return dest;
	while (*src) {
		*dest = isupper (*src) ? *src + 32 : *src;
		src++;
		dest++;
	}
	*dest = 0;
	return dest;
}

/**
 * Convert uppercase ASCII characters the nul-terminated string \src to
 * lowercase and store them in \dest. The terminating nul byte in \dest is
 * written. When \src or \dest are `nullptr`s, no action is performed. \dest
 * must have enough space to store all of \src, including the terminating nul
 * byte.
 */
char *
e_cstr_to_ascii_upper_buf (const char *restrict src, char *restrict dest)
{
	if (!src || !dest) return dest;
	while (*src) {
		*dest = islower (*src) ? *src - 32 : *src;
		src++;
		dest++;
	}
	*dest = 0;
	return dest;
}

/**
 * Find the character \c in the nul-terminated string \s. If no match is found
 * within the string, `nullptr` is returned. Otherwise, a pointer to the matched
 * character is returned. If \s is `nullptr`, `nullptr` is returned. This is
 * essentially like `strchr`, except that it allows for `nullptr` (which would
 * cause undefined behaviour in `strchr`).
 */
const char *
e_cstr_find_char (const char *s, char c)
{
	if (!s) return s;
	return strchr (s, (int) c);
}

/**
 * Find the first character which is not \c in the nul-terminated string \s. If
 * no match is found within the string, `nullptr` is returned. Otherwise, a
 * pointer to the matched character is returned. If \s is `nullptr`, `nullptr`
 * is returned.
 */
const char *
e_cstr_find_char_not (const char *s, char c)
{
	if (!s) return s;

	while (*s) {
		if (*s != c) return s;
		s++;
	}

	return NULL;
}

/**
 * Find the first character in the nul-terminated string \s which is also
 * contained in the nul-terminated string \accept. If no match is found within
 * the string, `nullptr` is returned. Otherwise, a pointer to the matched
 * character is returned. If \s or \accept are `nullptr`, `nullptr` is returned.
 */
const char *
e_cstr_find_char_pat (const char *s, const char *accept)
{
	if (!s || !accept) return NULL;

	while (*s) {
		if (strchr (accept, *s)) return s;
		s++;
	}

	return NULL;
}

/**
 * Find the first character in the nul-terminated string \s which is not
 * contained in the nul-terminated string \reject. If no match is found within
 * the string, `nullptr` is returned. Otherwise, a pointer to the matched
 * character is returned. If \s or \reject are `nullptr`, \s is returned.
 */
const char *
e_cstr_find_char_not_pat (const char *s, const char *reject)
{
	if (!s || !reject) return s;

	while (*s) {
		if (!strchr (reject, *s)) return s;
		s++;
	}

	return NULL;
}

/**
 * Find the first character in the nul-terminated string \s which matches the
 * predicate \func. If no match is found within the string, `nullptr` is
 * returned. Otherwise, a pointer to the matched character is returned. If \s or
 * \func are `nullptr`, `nullptr` is returned.
 */
const char *
e_cstr_find_char_func (const char *s, E_Char_Predicate func)
{
	if (!s || !func) return s;

	while (*s) {
		if (func (*s)) return s;
		s++;
	}

	return NULL;
}

/**
 * Find the first character in the nul-terminated string \s which is rejected by
 * the predicate \func. If no match is found within the string, `nullptr` is
 * returned. Otherwise, a pointer to the matched character is returned. If \s is
 * `nullptr`, `nullptr` is returned. If \func is `nullptr`, \s is returned.
 */
const char *
e_cstr_find_char_not_func (const char *s, E_Char_Predicate func)
{
	if (!s || !func) return s;

	while (*s) {
		if (!func (*s)) return s;
		s++;
	}

	return NULL;
}

/**
 * Find the substring \needle in the nul-terminated string \haystack. If no
 * match is found within the string, `nullptr` is returned. Otherwise, a pointer
 * to the matched substring is returned. If \haystack is `nullptr`, `nullptr` is
 * returned. If \needle is `nullptr`, \haystack is returned. This is essentially
 * like `strstr`, except that it allows for `nullptr` (which would cause
 * undefined behaviour in `strstr`).
 */
const char *
e_cstr_find_str (const char *haystack, const char *needle)
{
	if (!haystack || !needle) return haystack;
	return strstr (haystack, needle);
}

/**
 * Find the character \c in the nul-terminated string \s starting from the end
 * of the string. If no match is found within the string, `nullptr` is returned.
 * Otherwise, a pointer to the matched character is returned. If \s is
 * `nullptr`, `nullptr` is returned. This is essentially like `strrchr`, except
 * that it allows for `nullptr` (which would cause undefined behaviour in
 * `strrchr`).
 */
const char *
e_cstr_rfind_char (const char *s, char c)
{
	if (!s) return s;
	return strrchr (s, (int) c);
}

/**
 * Find the last character which is not \c in the nul-terminated string \s. If
 * no match is found within the string, `nullptr` is returned. Otherwise, a
 * pointer to the matched character is returned. If \s is `nullptr`, `nullptr`
 * is returned.
 */
const char *
e_cstr_rfind_char_not (const char *s, char c)
{
	const char *ret;

	if (!s) return s;

	ret = NULL;
	while (*s) {
		if (*s != c) ret = s;
		s++;
	}

	return ret;
}

/**
 * Find the last character in the nul-terminated string \s which is also
 * contained in the nul-terminated string \accept. If no match is found within
 * the string, `nullptr` is returned. Otherwise, a pointer to the matched
 * character is returned. If \s or \accept are `nullptr`, `nullptr` is returned.
 */
const char *
e_cstr_rfind_char_pat (const char *s, const char *accept)
{
	const char *ret;

	if (!s || !accept) return NULL;

	ret = NULL;
	while (*s) {
		if (strchr (accept, *s)) ret = s;
		s++;
	}

	return ret;
}

/**
 * Find the last character in the nul-terminated string \s which is not
 * contained in the nul-terminated string \reject. If no match is found within
 * the string, `nullptr` is returned. Otherwise, a pointer to the matched
 * character is returned. If \s or \reject are `nullptr`, \s is returned.
 */
const char *
e_cstr_rfind_char_not_pat (const char *s, const char *reject)
{
	const char *ret;

	if (!s || !reject) return s;

	ret = NULL;
	while (*s) {
		if (!strchr (reject, *s)) ret = s;
		s++;
	}

	return ret;
}

/**
 * Find the last character in the nul-terminated string \s which matches the
 * predicate \func. If no match is found within the string, `nullptr` is
 * returned. Otherwise, a pointer to the matched character is returned. If \s or
 * \func are `nullptr`, `nullptr` is returned.
 */
const char *
e_cstr_rfind_char_func (const char *s, E_Char_Predicate func)
{
	const char *ret;

	if (!s || !func) return s;

	ret = NULL;
	while (*s) {
		if (func (*s)) ret = s;
		s++;
	}

	return ret;
}

/**
 * Find the last character in the nul-terminated string \s which is rejected by
 * the predicate \func. If no match is found within the string, `nullptr` is
 * returned. Otherwise, a pointer to the matched character is returned. If \s is
 * `nullptr`, `nullptr` is returned. If \func is `nullptr`, \s is returned.
 */
const char *
e_cstr_rfind_char_not_func (const char *s, E_Char_Predicate func)
{
	const char *ret;

	if (!s || !func) return s;

	ret = NULL;
	while (*s) {
		if (!func (*s)) ret = s;
		s++;
	}

	return ret;
}

/**
 * Find the last substring \needle in the nul-terminated string \haystack. If no
 * match is found within the string, `nullptr` is returned. Otherwise, a pointer
 * to the matched substring is returned. If \haystack is `nullptr`, `nullptr` is
 * returned. If \needle is `nullptr`, \haystack is returned.
 */
const char *
e_cstr_rfind_str (const char *haystack, const char *needle)
{
	size_t haystack_len, needle_len;

	if (!haystack || !needle) return haystack;

	haystack_len = strlen (haystack);
	needle_len = strlen (needle);
	if (haystack_len == 0 || needle_len > haystack_len) return NULL;
	if (needle_len == 0) return &haystack[haystack_len - 1];

	haystack_len -= needle_len;
	for (;;) {
		if (!strncmp (&haystack[haystack_len], needle, needle_len)) {
			return &haystack[haystack_len];
		}
		if (haystack_len == 0) break;
		haystack_len -= 1;
	}

	return NULL;
}

/**
 * Check if the nul-terminated string \haystack contains the nul-terminated
 * string \needle. If \haystack is `nullptr`, `false` is returned. If \needle is
 * `nullptr`, `true` is returned.
 */
bool
e_cstr_contains (const char *haystack, const char *needle)
{
	return e_cstr_find_str (haystack, needle) != NULL;
}

/**
 * Checks if two strings \a and \b are equal. If both \a and \b are `nullptr`,
 * `true` is returned. If either \a or \b is a `nullptr`, `false` is returned.
 * Otherwise, `strcmp` is used to check if they are equal. This is essentially
 * like using `strcmp() == 0`, except that it allows for `nullptr` (which would
 * cause undefined behaviour in `strcmp`).
 */
bool
e_cstr_eq (const char *a, const char *b)
{
	if (a == b) return true;
	if (!a || !b) return false;
	return !strcmp (a, b);
}

/**
 * Checks if two strings \a and \b are equal in the first \n bytes. If both \a
 * and \b are `nullptr`, `true` is returned. If either \a or \b is a `nullptr`,
 * `false` is returned. Otherwise, `strncmp` is used to check if they are equal.
 * This is essentially like using `strncmp() == 0`, except that it allows for
 * `nullptr` (which would cause undefined behaviour in `strncmp`).
 */
bool
e_cstr_eq_n (const char *a, const char *b, size_t n)
{
	if (a == b) return true;
	if (!a || !b) return false;
	return !strncmp (a, b, n);
}

/**
 * Trim whitespace from the start of the nul-terminated string \s. Returns a
 * pointer to the start of the trimmed string. If \s only consists of
 * whitespace, the return value will point to the nul terminator. If `s` is
 * `nullptr`, `nullptr` is returned.
 */
const char *
e_cstr_trim_start (const char *s)
{
	if (!s) return s;
	while (isspace (*s)) s += 1;
	return s;
}

/**
 * Trim whitespace at the end of the nul-terminated string \s. Returns the
 * length of the trimmed string. If \s is `nullptr`, 0 is returned.
 */
size_t
e_cstr_trim_end (const char *s)
{
	if (!s) return 0;
	return e_cstr_trim_end_with_len (s, strlen (s));
}

/**
 * Trim whitespace at the end of the nul-terminated string \s with a given
 * length \len. \len must be equal to `strlen (s)`. Returns the length of the
 * trimmed string. If \s is `nullptr`, 0 is returned.
 */
size_t
e_cstr_trim_end_with_len (const char *s, size_t len)
{
	if (!s) return 0;
	while (len > 0 && isspace (s[len - 1])) len -= 1;
	return len;
}

/**
 * Trim whitespace at the start and end of the nul-terminated string \s. Returns
 * a pointer to the start of the trimmed string. \len will be set to the length
 * of the trimmed string. If \len is `nullptr`, it will not be filled and this
 * call is essentially equivalent to `e_cstr_trim_start`. If \s is `nullptr`,
 * `nullptr` is returned and \len is set to 0.
 */
const char *
e_cstr_trim (const char *s, size_t *len)
{
	size_t l;

	if (!len) return e_cstr_trim_start (s);
	if (!s) {
		*len = 0;
		return s;
	}

	l = strlen (s);
	s = e_cstr_trim_with_len (s, &l);
	*len = l;

	return s;
}

/**
 * Trim whitespace at the start and end of the nul-terminated string \s of
 * length \len. \len must be equal to `strlen (s)`. Returns a pointer to the
 * start of the trimmed string. \len will be set to the length of the trimmed
 * string. If \len is `nullptr`, it will not be filled and this call is
 * essentially equivalent to `e_cstr_trim_start`. If \s is `nullptr`, `nullptr`
 * is returned and \len is set to 0.
 */
const char *
e_cstr_trim_with_len (const char *s, size_t *len)
{
	const char *start;
	size_t l;

	if (!len) return e_cstr_trim_start (s);
	if (!s) {
		*len = 0;
		return s;
	}

	start = e_cstr_trim_start (s);
	l = *len - (start - s);
	l = e_cstr_trim_end_with_len (start, l);
	*len = l;

	return start;
}

/**
 * Check if a nul-terminated string \s starts with the nul-terminated string
 * \expect. If \s is `nullptr`, `false` is returned. If \expect is `nullptr`,
 * `true` is returned.
 */
bool
e_cstr_starts_with (const char *s, const char *expect)
{
	if (!s) return false;
	if (!expect) return true;
	return e_cstr_eq_n (s, expect, strlen (expect));
}

/**
 * Check if a nul-terminated string \s ends with the nul-terminated string
 * \expect. If \s is `nullptr`, `false` is returned. If \expect is `nullptr`,
 * `true` is returned.
 */
bool
e_cstr_ends_with (const char *s, const char *expect)
{
	size_t s_len, expect_len;

	if (!s) return false;
	if (!expect) return true;
	s_len = strlen (s);
	expect_len = strlen (expect);
	if (expect_len > s_len) return false;
	return e_cstr_eq (s + s_len - expect_len, expect);
}

/**
 * Check if a nul-terminated string \s continues with the nul-terminated string
 * \expect at the position \pos. If \s is `nullptr` or \pos is outside of the
 * string, `false` is returned. If \expect is `nullptr`, `true` is returned.
 */
bool
e_cstr_continues_with (const char *s, const char *expect, size_t pos)
{
	if (!s) return false;
	if (!expect) return true;
	if (pos > strlen (s)) return false;
	return e_cstr_eq_n (s + pos, expect, strlen (expect));
}

#define __MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

/**
 * Calculate the Levenshtein distance (i.e. the number of simple edits to
 * transform \a to \b) between two nul-terminated strings \a and \b. If \a
 * or \b are `nullptr`, they will be treated as if they were empty strings.
 */
size_t
e_cstr_distance (const char *a, const char *b)
{
	size_t a_len, b_len, i, j, last_diag, old_diag, sub, *col, alloc_size;

	if (!a && !b) return 0;
	if (!a) return strlen (b);
	if (!b) return strlen (a);

	a_len = strlen (a);
	b_len = strlen (b);
	alloc_size = sizeof (size_t) * (a_len + 1);
	col = E_CONFIG_MALLOC_FUNC (alloc_size);
	if (!col) {
		fprintf (stderr, "[e_cstr] failed to alloc %zu bytes\n", alloc_size);
		exit (EXIT_FAILURE);
	}

	for (i = 0; i <= a_len; i++) col[i] = i;
	for (i = 1; i <= b_len; i++) {
		col[0] = i;
		last_diag = i - 1;
		for (j = 1; j <= a_len; j++) {
			old_diag = col[j];
			sub = last_diag + (a[j - 1] == b[i - 1] ? 0 : 1);
			col[j] = __MIN3 (col[j] + 1, col[j - 1] + 1, sub);
			last_diag = old_diag;
		}
	}

	i = col[a_len];
	E_CONFIG_FREE_FUNC (col);
	return i;
}

/**
 * Split a nul-terminated string \s at the character \c. Returns a `E_Str_Split`
 * iterator that can be used for examining the split up string. The returned
 * iterator must be freed with `e_cstr_split_deinit`.
 *
 * Example usage:
 *  | E_Str_Split split = e_cstr_split_char ("foo/bar/baz", '/');
 *  | const char *part1 = e_cstr_split_next (&split);
 *  | const char *part2 = e_cstr_split_next (&split);
 *  | const char *part3 = e_cstr_split_next (&split);
 *  | e_cstr_split_deinit (&split);
 */
E_Str_Split
e_cstr_split_char (const char *s, char c)
{
	char *buf, *bufiter, *pos;
	size_t count, len;

	if (!s) return (E_Str_Split) {0};

	count = 1;
	len = strlen (s);
	buf = E_CONFIG_MALLOC_FUNC (sizeof (char) * (len + 1));
	if (!buf) {
		fprintf (stderr, "[e_cstr] failed to alloc %zu bytes\n", len + 1);
		exit (EXIT_FAILURE);
	}
	strcpy (buf, s);
	bufiter = buf;
	while ((pos = (char *) e_cstr_find_char (bufiter, c))) {
		*pos = 0;
		bufiter = pos + 1;
		count += 1;
	}

	return (E_Str_Split) {
		.num_items = count,
		.next_item = 0,
		.buf = buf,
		.next = buf,
	};
}

/**
 * Split a nul-terminated string \s at the delimiter given by the nul-terminated
 * string \delim. Returns a `E_Str_Split` iterator that can be used for
 * examining the split up string. The returned iterator must be freed with
 * `e_cstr_split_deinit`. See `e_cstr_split_cstr` for an example.
 */
E_Str_Split
e_cstr_split_str (const char *s, const char *delim)
{
	size_t count, size, offset, len, delim_len;
	char *buf, *bufiter, *pos;

	if (!s) return (E_Str_Split) {0};

	if (!delim || strlen (delim) == 0) {
		buf = E_CONFIG_MALLOC_FUNC (sizeof (char));
		if (!buf) {
			fprintf (stderr, "[e_cstr] failed to alloc 1 byte\n");
			exit (EXIT_FAILURE);
		}
		buf[0] = 0;
		return (E_Str_Split) {
			.num_items = 1,
			.next_item = 0,
			.buf = buf,
			.next = buf,
		};
	}

	delim_len = strlen (delim);
	count = e_cstr_count_str (s, delim);
	size = strlen (s) - count * delim_len + count + 1;
	buf = E_CONFIG_MALLOC_FUNC (sizeof (char) * size);
	if (!buf) {
		fprintf (stderr, "[e_cstr] failed to alloc %zu bytes\n", size);
		exit (EXIT_FAILURE);
	}
	bufiter = buf;
	count = 0;
	offset = 0;
	while ((pos = (char *) e_cstr_find_str (s + offset, delim))) {
		len = pos - (s + offset);
		memcpy (bufiter, s + offset, sizeof (char) * len);
		bufiter[len] = 0;
		offset = pos - s + delim_len;
		bufiter += len + 1;
		count += 1;
	}
	strcpy (bufiter, s + offset);
	count += 1;

	return (E_Str_Split) {
		.num_items = count,
		.next_item = 0,
		.buf = buf,
		.next = buf,
	};
}

/**
 * Split a nul-terminated string \s at newline characters. Returns a
 * `E_Str_Split` iterator that can be used for examining the split up string.
 * The returned iterator must be freed with `e_cstr_split_deinit`. See
 * `e_cstr_split_char` for an example.
 */
E_Str_Split
e_cstr_split_lines (const char *s)
{
	return e_cstr_split_char (s, '\n');
}

/**
 * Get the next item in the string split iterator \split. If no item is found,
 * `nullptr` is returned. Note that since splitting up a string requires
 * allocating new memory for the split items, the returned pointer is NOT part
 * of the original string that was split up.
 */
const char *
e_cstr_split_next (E_Str_Split *split)
{
	const char *ret;

	if (!split || !split->buf || !split->next) return NULL;
	if (split->next_item + 1 > split->num_items) return NULL;
	ret = split->next;
	split->next_item += 1;
	split->next += strlen (split->next) + 1;

	return ret;
}

/**
 * Deinitialize a string split iterator \split and free its occupied memory.
 */
void
e_cstr_split_deinit (E_Str_Split *split)
{
	if (split) E_CONFIG_FREE_FUNC (split->buf);
}

/**
 * Check whether a character \c is ASCII or not. This is the same as `isascii`
 * from POSIX.
 */
int
e_char_is_ascii (int c)
{
	return 0 <= c && c <= 127;
}

#endif /* E_CSTR_IMPL */

#endif /* _EMPOWER_CSTR_H_ */
