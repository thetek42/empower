#include "e_cstr.h"

#if E_CONFIG_MODULE_CSTR

#include "convey.h"
#include <ctype.h>
#include <string.h>
#include "e_mem.h"

/**
 * Count the number of occurances of a character \c in the nul-terminated string
 * \s. When \s is `nullptr`, it returns 0.
 */
usize
e_cstr_count_char (const char *s, char c)
{
	usize r;

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
usize
e_cstr_count_char_not (const char *s, char c)
{
	usize r;

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
usize
e_cstr_count_char_pat (const char *s, const char *accept)
{
	usize r;

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
usize
e_cstr_count_char_not_pat (const char *s, const char *reject)
{
	usize r;

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
usize
e_cstr_count_char_func (const char *s, E_Char_Predicate func)
{
	usize r;

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
usize
e_cstr_count_char_not_func (const char *s, E_Char_Predicate func)
{
	usize r;

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
usize
e_cstr_count_str (const char *haystack, const char *needle)
{
	usize haystack_len, needle_len, count;

	if (!haystack || !needle) return 0;

	haystack_len = strlen (haystack);
	needle_len = strlen (needle);
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
usize
e_cstr_count_str_overlap (const char *haystack, const char *needle)
{
	usize haystack_len, needle_len, count;

	if (!haystack || !needle) return 0;

	haystack_len = strlen (haystack);
	needle_len = strlen (needle);
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
usize
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
usize e_cstr_len (const char *s)
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
	return e_cstr_matches_predicate (s, c_isascii);
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

	return nullptr;
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
	if (!s || !accept) return nullptr;

	while (*s) {
		if (strchr (accept, *s)) return s;
		s++;
	}

	return nullptr;
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

	return nullptr;
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

	return nullptr;
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

	return nullptr;
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

	ret = nullptr;
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

	if (!s || !accept) return nullptr;

	ret = nullptr;
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

	ret = nullptr;
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

	ret = nullptr;
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

	ret = nullptr;
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
	usize haystack_len, needle_len;

	if (!haystack || !needle) return haystack;

	haystack_len = strlen (haystack);
	needle_len = strlen (needle);
	if (haystack_len == 0 || needle_len > haystack_len) return nullptr;
	if (needle_len == 0) return &haystack[haystack_len - 1];

	haystack_len -= needle_len;
	for (;;) {
		if (!strncmp (&haystack[haystack_len], needle, needle_len)) {
			return &haystack[haystack_len];
		}
		if (haystack_len == 0) break;
		haystack_len -= 1;
	}

	return nullptr;
}

/**
 * Check if the nul-terminated string \haystack contains the nul-terminated
 * string \needle. If \haystack is `nullptr`, `false` is returned. If \needle is
 * `nullptr`, `true` is returned.
 */
bool
e_cstr_contains (const char *haystack, const char *needle)
{
	return e_cstr_find_str (haystack, needle) != nullptr;
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
e_cstr_eq_n (const char *a, const char *b, usize n)
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
usize
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
usize
e_cstr_trim_end_with_len (const char *s, usize len)
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
e_cstr_trim (const char *s, usize *len)
{
	usize l;

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
e_cstr_trim_with_len (const char *s, usize *len)
{
	const char *start;
	usize l;

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
	usize s_len, expect_len;

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
e_cstr_continues_with (const char *s, const char *expect, usize pos)
{
	if (!s) return false;
	if (!expect) return true;
	if (pos > strlen (s)) return false;
	return e_cstr_eq_n (s + pos, expect, strlen (expect));
}

#if E_CONFIG_MODULE_ALLOC
#define __MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

/**
 * Calculate the Levenshtein distance (i.e. the number of simple edits to
 * transform \a to \b) between two nul-terminated strings \a and \b. If \a
 * or \b are `nullptr`, they will be treated as if they were empty strings.
 */
usize
e_cstr_distance (const char *a, const char *b)
{
	usize a_len, b_len, i, j, last_diag, old_diag, sub, *col;

	if (!a && !b) return 0;
	if (!a) return strlen (b);
	if (!b) return strlen (a);

	a_len = strlen (a);
	b_len = strlen (b);
	col = e_alloc (usize, a_len + 1);

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
	e_free (col);
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
	usize count;

	if (!s) return (E_Str_Split) {0};

	count = 1;
	buf = e_mem_strdup (s);
	bufiter = buf;
	while ((pos = (char *) e_cstr_find_char (bufiter, c))) {
		*pos = 0;
		bufiter = pos + 1;
		count += 1;
	}

	return (E_Str_Split) {
		.num_items = count,
		.__next_item = 0,
		.__buf = buf,
		.__next = buf,
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
	usize count, size, offset, len, delim_len;
	char *buf, *bufiter, *pos;

	if (!s) return (E_Str_Split) {0};

	if (!delim || strlen (delim) == 0) {
		buf = e_mem_strdup (s);
		return (E_Str_Split) {
			.num_items = 1,
			.__next_item = 0,
			.__buf = buf,
			.__next = buf,
		};
	}

	delim_len = strlen (delim);
	count = e_cstr_count_str (s, delim);
	size = strlen (s) - count * delim_len + count + 1;
	buf = e_alloc (char, size);
	bufiter = buf;
	count = 0;
	offset = 0;
	while ((pos = (char *) e_cstr_find_str (s + offset, delim))) {
		len = pos - (s + offset);
		e_mem_copy (bufiter, s + offset, char, len);
		bufiter[len] = 0;
		offset = pos - s + delim_len;
		bufiter += len + 1;
		count += 1;
	}
	strcpy (bufiter, s + offset);
	count += 1;

	return (E_Str_Split) {
		.num_items = count,
		.__next_item = 0,
		.__buf = buf,
		.__next = buf,
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

	if (!split || !split->__buf || !split->__next) return nullptr;
	if (split->__next_item + 1 > split->num_items) return nullptr;
	ret = split->__next;
	split->__next_item += 1;
	split->__next += strlen (split->__next) + 1;

	return ret;
}

/**
 * Deinitialize a string split iterator \split and free its occupied memory.
 */
void
e_cstr_split_deinit (E_Str_Split *split)
{
	if (split) e_free (split->__buf);
}

#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_CSTR */
