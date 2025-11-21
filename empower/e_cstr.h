#ifndef EMPOWER_CSTR_H_
#define EMPOWER_CSTR_H_

/*! e_cstr ********************************************************************
 *
 * This module provides various utility functions for dealing with C strings and
 * serves as an extension to the `string.h` header from the standard library.
 *
 ******************************************************************************/

#include <stddef.h>
#include <stdint.h>

/**
 * A pointer to a function that evaulates whether the ASCII character `c`
 * matches a certain predicate. It returns 0 if `c` is accepted, and non-zero
 * if it is rejected. The functions from `<ctype.h>` have this type.
 */
typedef int (* e_char_predicate_t) (int c);

size_t e_cstr_count_char (const char *s, char c);
size_t e_cstr_count_char_not (const char *s, char c);
size_t e_cstr_count_char_pat (const char *s, const char *accept);
size_t e_cstr_count_char_not_pat (const char *s, const char *reject);
size_t e_cstr_count_char_func (const char *s, e_char_predicate_t func);
size_t e_cstr_count_char_not_func (const char *s, e_char_predicate_t func);
size_t e_cstr_count_str (const char *haystack, const char *needle);
size_t e_cstr_count_str_overlap (const char *haystack, const char *needle);
size_t e_cstr_count_lines (const char *s);
size_t e_cstr_len (const char *s);
int e_cstr_is_ascii (const char *s);
int e_cstr_is_blank (const char *s);
int e_cstr_matches_predicate (const char *s, e_char_predicate_t func);
char *e_cstr_to_ascii_lower (char *s);
char *e_cstr_to_ascii_upper (char *s);
char *e_cstr_to_ascii_lower_buf (const char *src, char *dest);
char *e_cstr_to_ascii_upper_buf (const char *src, char *dest);
const char *e_cstr_find_char (const char *s, char c);
const char *e_cstr_find_char_not (const char *s, char c);
const char *e_cstr_find_char_pat (const char *s, const char *accept);
const char *e_cstr_find_char_not_pat (const char *s, const char *reject);
const char *e_cstr_find_char_func (const char *s, e_char_predicate_t func);
const char *e_cstr_find_char_not_func (const char *s, e_char_predicate_t func);
const char *e_cstr_find_str (const char *haystack, const char *needle);
const char *e_cstr_rfind_char (const char *s, char c);
const char *e_cstr_rfind_char_not (const char *s, char c);
const char *e_cstr_rfind_char_pat (const char *s, const char *accept);
const char *e_cstr_rfind_char_not_pat (const char *s, const char *reject);
const char *e_cstr_rfind_char_func (const char *s, e_char_predicate_t func);
const char *e_cstr_rfind_char_not_func (const char *s, e_char_predicate_t func);
const char *e_cstr_rfind_str (const char *haystack, const char *needle);
int e_cstr_contains (const char *haystack, const char *needle);
int e_cstr_eq (const char *a, const char *b);
int e_cstr_eq_n (const char *a, const char *b, size_t n);
const char *e_cstr_trim_start (const char *s);
size_t e_cstr_trim_end (const char *s);
size_t e_cstr_trim_end_with_len (const char *s, size_t len);
const char *e_cstr_trim (const char *s, size_t *len);
const char *e_cstr_trim_with_len (const char *s, size_t *len);
int e_cstr_starts_with (const char *s, const char *expect);
int e_cstr_ends_with (const char *s, const char *expect);
int e_cstr_continues_with (const char *s, const char *expect, size_t pos);
size_t e_cstr_distance (const char *a, const char *b);
int e_char_is_ascii (int c);

/******************************************************************************/

#ifdef E_CSTR_IMPL

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Count the number of occurances of a character `c` in the nul-terminated
 * string `s`.
 */
size_t
e_cstr_count_char (const char *s,
                   char c)
{
	size_t r;

	r = 0;
	while (*s) {
		if (*s++ == c) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters which are not `c` in the
 * nul-terminated string `s`.
 */
size_t
e_cstr_count_char_not (const char *s,
                       char c)
{
	size_t r;

	r = 0;
	while (*s) {
		if (*s++ != c) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters in the nul-terminated string `s`
 * which are contained in the nul-terminated string `accept`.
 */
size_t
e_cstr_count_char_pat (const char *s,
                       const char *accept)
{
	size_t r;

	r = 0;
	while (*s) {
		if (strchr (accept, *s++)) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters in the nul-terminated string `s`
 * which are not contained in the nul-terminated string `reject`.
 */
size_t
e_cstr_count_char_not_pat (const char *s,
                           const char *reject)
{
	size_t r;

	r = 0;
	while (*s) {
		if (!strchr (reject, *s++)) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters in the nul-terminated string `s`
 * which are accepted by the e_char_predicate_t `func`.
 */
size_t
e_cstr_count_char_func (const char *s,
                        e_char_predicate_t func)
{
	size_t r;

	r = 0;
	while (*s) {
		if (func (*s++)) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of characters in the nul-terminated string `s`
 * which are rejected by the e_char_predicate_t `func`.
 */
size_t
e_cstr_count_char_not_func (const char *s,
                            e_char_predicate_t func)
{
	size_t r;

	r = 0;
	while (*s) {
		if (!func (*s++)) r += 1;
	}
	return r;
}

/**
 * Count the number of occurances of a nul-terminated string `needle` in the
 * nul-terminated string `haystack`. Overlap is not counted, so searching for
 * "aa" in the string "aaa" would yield 1.
 */
size_t
e_cstr_count_str (const char *haystack,
                  const char *needle)
{
	size_t haystack_len, needle_len, count;

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
 * Count the number of occurances of a nul-terminated string `needle` in the
 * nul-terminated string `haystack`. Overlap is counted, so searching for "aa"
 * in the string "aaa" would yield 2.
 */
size_t
e_cstr_count_str_overlap (const char *haystack,
                          const char *needle)
{
	size_t haystack_len, needle_len, count;

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
 * Count the number of lines in the nul-terminated string `s`.
 */
size_t
e_cstr_count_lines (const char *s)
{
	return 1 + e_cstr_count_char (s, '\n');
}

/**
 * Get the length of a string `s`. If `s` is NULL, 0 is returned. This is
 * essentially like `strlen()`, except that it allows for NULL (which would
 * cause undefined behaviour in `strlen()`)
 */
size_t
e_cstr_len (const char *s)
{
	return s ? strlen (s) : 0;
}

/**
 * Check if the nul-terminated string `s` is ASCII. Returns non-zero when `s`
 * only consists of ASCII characters.
 */
int
e_cstr_is_ascii (const char *s)
{
	return e_cstr_matches_predicate (s, e_char_is_ascii);
}

/**
 * Check if the nul-terminated string `s` only consists of whitespace characters
 * as per `isspace`. Returns non-zero when `s` only consists of whitespace.
 */
int
e_cstr_is_blank (const char *s)
{
	return e_cstr_matches_predicate (s, isspace);
}

/**
 * Check if the nul-terminated string `s` matches the e_char_predicate_t `func`.
 * Returns non-zero when `s` only consists of characters that were accepted by
 * `func`.
 */
int
e_cstr_matches_predicate (const char *s,
                          e_char_predicate_t func)
{
	while (*s) {
		if (!func (*s++)) return 0;
	}
	return 1;
}

/**
 * Convert uppercase ASCII characters the nul-terminated string `s` to lowercase
 * in-place. Returns `s`.
 */
char *
e_cstr_to_ascii_lower (char *s)
{
	char *p;
	p = s;
	while (*p) {
		*p = (char) tolower (*p);
		p += 1;
	}
	return s;
}

/**
 * Convert lowercase ASCII characters the nul-terminated string `s` to uppercase
 * in-place. Returns `s`.
 */
char *
e_cstr_to_ascii_upper (char *s)
{
	char *p;
	p = s;
	while (*p) {
		*p = (char) toupper (*p);
		p += 1;
	}
	return s;
}

/**
 * Convert uppercase ASCII characters the nul-terminated string `src` to
 * lowercase and store them in `dest`. `dest` must have enough space to store
 * all of `src`, including the terminating nul byte. Returns `dest`.
 */
char *
e_cstr_to_ascii_lower_buf (const char *src,
                           char *dest)
{
	while (*src) {
		*dest = isupper (*src) ? *src + 32 : *src;
		src++;
		dest++;
	}
	*dest = 0;
	return dest;
}

/**
 * Convert uppercase ASCII characters the nul-terminated string `src` to
 * lowercase and store them in `dest`. `dest` must have enough space to store
 * all of `src`, including the terminating nul byte. Returns `dest`.
 */
char *
e_cstr_to_ascii_upper_buf (const char *src,
                           char *dest)
{
	while (*src) {
		*dest = islower (*src) ? *src - 32 : *src;
		src++;
		dest++;
	}
	*dest = 0;
	return dest;
}

/**
 * Find the character `c` in the nul-terminated string `s`. If no match is found
 * within the string, NULL is returned. Otherwise, a pointer to the matched
 * character is returned. If `s` is NULL, NULL is returned. This is basically
 * just a wrapper for `strchr()`.
 */
const char *
e_cstr_find_char (const char *s,
                  char c)
{
	return strchr (s, (int) c);
}

/**
 * Find the first character which is not `c` in the nul-terminated string `s`.
 * If no match is found within the string, NULL is returned. Otherwise, a
 * pointer to the matched character is returned.
 */
const char *
e_cstr_find_char_not (const char *s,
                      char c)
{
	while (*s) {
		if (*s != c) return s;
		s++;
	}
	return NULL;
}

/**
 * Find the first character in the nul-terminated string `s` which is also
 * contained in the nul-terminated string `accept`. If no match is found within
 * the string, NULL is returned. Otherwise, a pointer to the matched character
 * is returned.
 */
const char *
e_cstr_find_char_pat (const char *s,
                      const char *accept)
{
	while (*s) {
		if (strchr (accept, *s)) return s;
		s++;
	}
	return NULL;
}

/**
 * Find the first character in the nul-terminated string `s` which is not
 * contained in the nul-terminated string `reject`. If no match is found within
 * the string, NULL is returned. Otherwise, a pointer to the matched character
 * is returned.
 */
const char *
e_cstr_find_char_not_pat (const char *s,
                          const char *reject)
{
	while (*s) {
		if (!strchr (reject, *s)) return s;
		s++;
	}
	return NULL;
}

/**
 * Find the first character in the nul-terminated string `s` which matches the
 * predicate `func`. If no match is found within the string, NULL is returned.
 * Otherwise, a pointer to the matched character is returned.
 */
const char *
e_cstr_find_char_func (const char *s,
                       e_char_predicate_t func)
{
	while (*s) {
		if (func (*s)) return s;
		s++;
	}
	return NULL;
}

/**
 * Find the first character in the nul-terminated string `s` which is rejected
 * by the predicate `func`. If no match is found within the string, NULL is
 * returned. Otherwise, a pointer to the matched character is returned.
 */
const char *
e_cstr_find_char_not_func (const char *s,
                           e_char_predicate_t func)
{
	while (*s) {
		if (!func (*s)) return s;
		s++;
	}
	return NULL;
}

/**
 * Find the substring `needle` in the nul-terminated string `haystack`. If no
 * match is found within the string, NULL is returned. Otherwise, a pointer to
 * the matched substring is returned. This is merely a wrapper for `strstr()`.
 */
const char *
e_cstr_find_str (const char *haystack,
                 const char *needle)
{
	return strstr (haystack, needle);
}

/**
 * Find the character `c` in the nul-terminated string `s` starting from the end
 * of the string. If no match is found within the string, NULL is returned.
 * Otherwise, a pointer to the matched character is returned. This is merely a
 * wrapper for `strrchr()`.
 */
const char *
e_cstr_rfind_char (const char *s,
                   char c)
{
	return strrchr (s, (int) c);
}

/**
 * Find the last character which is not `c` in the nul-terminated string `s`.
 * If no match is found within the string, NULL is returned. Otherwise, a
 * pointer to the matched character is returned.
 */
const char *
e_cstr_rfind_char_not (const char *s,
                       char c)
{
	const char *ret;
	ret = NULL;
	while (*s) {
		if (*s != c) ret = s;
		s++;
	}
	return ret;
}

/**
 * Find the last character in the nul-terminated string `s` which is also
 * contained in the nul-terminated string `accept`. If no match is found within
 * the string, NULL is returned. Otherwise, a pointer to the matched character
 * is returned.
 */
const char *
e_cstr_rfind_char_pat (const char *s,
                       const char *accept)
{
	const char *ret;
	ret = NULL;
	while (*s) {
		if (strchr (accept, *s)) ret = s;
		s++;
	}
	return ret;
}

/**
 * Find the last character in the nul-terminated string `s` which is not
 * contained in the nul-terminated string `reject`. If no match is found within
 * the string, NULL is returned. Otherwise, a pointer to the matched character
 * is returned.
 */
const char *
e_cstr_rfind_char_not_pat (const char *s,
                           const char *reject)
{
	const char *ret;
	ret = NULL;
	while (*s) {
		if (!strchr (reject, *s)) ret = s;
		s++;
	}
	return ret;
}

/**
 * Find the last character in the nul-terminated string `s` which matches the
 * predicate `func`. If no match is found within the string, NULL is returned.
 * Otherwise, a pointer to the matched character is returned.
 */
const char *
e_cstr_rfind_char_func (const char *s,
                        e_char_predicate_t func)
{
	const char *ret;
	ret = NULL;
	while (*s) {
		if (func (*s)) ret = s;
		s++;
	}
	return ret;
}

/**
 * Find the last character in the nul-terminated string `s` which is rejected by
 * the predicate `func`. If no match is found within the string, NULL is
 * returned. Otherwise, a pointer to the matched character is returned.
 */
const char *
e_cstr_rfind_char_not_func (const char *s,
                            e_char_predicate_t func)
{
	const char *ret;
	ret = NULL;
	while (*s) {
		if (!func (*s)) ret = s;
		s++;
	}
	return ret;
}

/**
 * Find the last substring `needle` in the nul-terminated string `haystack`.
 * If no match is found within the string, NULL is returned. Otherwise, a
 * pointer to the matched substring is returned.
 */
const char *
e_cstr_rfind_str (const char *haystack,
                  const char *needle)
{
	size_t haystack_len, needle_len;

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
 * Check if the nul-terminated string `haystack` contains the nul-terminated
 * string `needle`. Returns non-zero if the string was found.
 */
int
e_cstr_contains (const char *haystack,
                 const char *needle)
{
	return e_cstr_find_str (haystack, needle) != NULL;
}

/**
 * Checks if two strings `a` and `b` are equal. If both `a` and `b` are NULL,
 * or the strings are equal, non-zero is returned. Otherwise, 0 is returned.
 * This is essentially like using `strcmp() == 0`, except that it allows for
 * NULL.
 */
int
e_cstr_eq (const char *a,
           const char *b)
{
	if (a == b) return 1;
	if (!a || !b) return 0;
	return !strcmp (a, b);
}

/**
 * Checks if two strings `a` and `b` are equal in the first `n` bytes. If both
 * `a` and `b` are NULL, or the strings are equal, non-zero is returned.
 * Otherwise, 0 is returned. This is essentially like using `strncmp() == 0`,
 * except that it allows for * NULL.
 */
int
e_cstr_eq_n (const char *a,
             const char *b,
             size_t n)
{
	if (a == b) return 1;
	if (!a || !b) return 0;
	return !strncmp (a, b, n);
}

/**
 * Trim whitespace from the start of the nul-terminated string `s`. Returns a
 * pointer to the start of the trimmed string. If `s` only consists of
 * whitespace, the return value will point to the nul terminator.
 */
const char *
e_cstr_trim_start (const char *s)
{
	while (isspace (*s)) s += 1;
	return s;
}

/**
 * Trim whitespace at the end of the nul-terminated string `s`. Returns the
 * length of the trimmed string.
 */
size_t
e_cstr_trim_end (const char *s)
{
	return e_cstr_trim_end_with_len (s, strlen (s));
}

/**
 * Trim whitespace at the end of the nul-terminated string `s` with a given
 * length `len`. `len` must be equal to `strlen (s)`. Returns the length of the
 * trimmed string.
 */
size_t
e_cstr_trim_end_with_len (const char *s,
                          size_t len)
{
	while (len > 0 && isspace (s[len - 1])) len -= 1; /* NOLINT */
	return len;
}

/**
 * Trim whitespace at the start and end of the nul-terminated string `s`.
 * Returns a pointer to the start of the trimmed string. `len` will be set to
 * the length of the trimmed string. If `len` is NULL, it will not be filled and
 * this call is essentially equivalent to `e_cstr_trim_start`.
 */
const char *
e_cstr_trim (const char *s,
             size_t *len)
{
	size_t l;
	if (!len) return e_cstr_trim_start (s);
	l = strlen (s);
	s = e_cstr_trim_with_len (s, &l);
	*len = l;
	return s;
}

/**
 * Trim whitespace at the start and end of the nul-terminated string `s` of
 * length `len`. `len` must be equal to `strlen (s)`. Returns a pointer to the
 * start of the trimmed string. `len` will be set to the length of the trimmed
 * string. If `len` is NULL, it will not be filled and this call is essentially
 * equivalent to `e_cstr_trim_start`.
 */
const char *
e_cstr_trim_with_len (const char *s,
                      size_t *len)
{
	const char *start;
	size_t l;

	if (!len) return e_cstr_trim_start (s);
	start = e_cstr_trim_start (s);
	l = *len - (size_t) (start - s);
	l = e_cstr_trim_end_with_len (start, l);
	*len = l;

	return start;
}

/**
 * Check if a nul-terminated string `s` starts with the nul-terminated string
 * `expect`. Returns non-zero if `s` starts with `expect`.
 */
int
e_cstr_starts_with (const char *s,
                    const char *expect)
{
	return e_cstr_eq_n (s, expect, strlen (expect));
}

/**
 * Check if a nul-terminated string `s` ends with the nul-terminated string
 * `expect`. Returns non-zero if `s` ends with `expect`.
 */
int
e_cstr_ends_with (const char *s,
                  const char *expect)
{
	size_t s_len, expect_len;
	s_len = strlen (s);
	expect_len = strlen (expect);
	if (expect_len > s_len) return 0;
	return e_cstr_eq (s + s_len - expect_len, expect);
}

/**
 * Check if a nul-terminated string `s` continues with the nul-terminated string
 * `expect` at the position `pos`. Returns non-zero if a match is found.
 */
int
e_cstr_continues_with (const char *s,
                       const char *expect,
                       size_t pos)
{
	if (pos > strlen (s)) return 0;
	return e_cstr_eq_n (s + pos, expect, strlen (expect));
}

#define E_CSTR_MIN3_(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

/**
 * Calculate the Levenshtein distance (i.e. the number of simple edits to
 * transform one string into another) between two nul-terminated strings `a` and
 * `b`. This function allocates memory internally. If the memory allocation
 * fails, `(size_t) -1` is returned.
 */
size_t
e_cstr_distance (const char *a,
                 const char *b)
{
	size_t a_len, b_len, i, j, last_diag, old_diag, sub, *col, alloc_size;

	a_len = strlen (a);
	b_len = strlen (b);
	alloc_size = sizeof (size_t) * (a_len + 1);
	col = malloc (alloc_size);
	if (!col) return (size_t) (-1);

	for (i = 0; i <= a_len; i++) col[i] = i;
	for (i = 1; i <= b_len; i++) {
		col[0] = i;
		last_diag = i - 1;
		for (j = 1; j <= a_len; j++) {
			old_diag = col[j];
			sub = last_diag + (a[j - 1] == b[i - 1] ? 0 : 1);
			col[j] = E_CSTR_MIN3_ (col[j] + 1, col[j - 1] + 1, sub);
			last_diag = old_diag;
		}
	}

	i = col[a_len];
	free (col);
	return i;
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

#endif /* EMPOWER_CSTR_H_ */
