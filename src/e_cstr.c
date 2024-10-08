#include "empower.h"

#if E_CONFIG_MODULE_CSTR

E_ATTR_REPRODUCIBLE
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

E_ATTR_REPRODUCIBLE
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

E_ATTR_REPRODUCIBLE
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

E_ATTR_REPRODUCIBLE
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

E_ATTR_REPRODUCIBLE
usize
e_cstr_count_lines (const char *s)
{
	if (!s) return 0;
	return 1 + e_cstr_count_char (s, '\n');
}

E_ATTR_REPRODUCIBLE
usize e_cstr_len (const char *s)
{
	return s ? strlen (s) : 0;
}

E_ATTR_REPRODUCIBLE
bool
e_cstr_is_ascii (const char *s)
{
	return e_cstr_matches_predicate (s, isascii);
}

E_ATTR_REPRODUCIBLE
bool
e_cstr_is_blank (const char *s)
{
	return e_cstr_matches_predicate (s, isspace);
}

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

char *
e_cstr_to_ascii_lower (char *s)
{
	char *p;

	p = s;
	while (s && *p) {
		if (isupper (*p))
			*p += 32;
		p++;
	}
	return s;
}

char *
e_cstr_to_ascii_upper (char *s)
{
	char *p;

	p = s;
	while (s && *p) {
		if (islower (*p))
			*p -= 32;
		p++;
	}
	return s;
}

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

E_ATTR_REPRODUCIBLE
const char *
e_cstr_find_char (const char *s, char c)
{
	if (!s) return s;
	return strchr (s, (int) c);
}

E_ATTR_REPRODUCIBLE
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

E_ATTR_REPRODUCIBLE
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

E_ATTR_REPRODUCIBLE
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

E_ATTR_REPRODUCIBLE
const char *
e_cstr_find_str (const char *haystack, const char *needle)
{
	if (!haystack || !needle) return haystack;
	return strstr (haystack, needle);
}

E_ATTR_REPRODUCIBLE
bool
e_cstr_contains (const char *haystack, const char *needle)
{
	return e_cstr_find_str (haystack, needle) != nullptr;
}

E_ATTR_REPRODUCIBLE
bool
e_cstr_eq (const char *a, const char *b)
{
	if (a == b) return true;
	if (!a || !b) return false;
	return !strcmp (a, b);
}

E_ATTR_REPRODUCIBLE
bool
e_cstr_eq_n (const char *a, const char *b, usize n)
{
	if (a == b) return true;
	if (!a || !b) return false;
	return !strncmp (a, b, n);
}

#if E_CONFIG_MODULE_ALLOC
#define __MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

E_ATTR_REPRODUCIBLE
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

#endif /* E_CONFIG_MODULE_ALLOC */

E_ATTR_REPRODUCIBLE
const char *
e_cstr_trim_start (const char *s)
{
	if (!s) return s;
	while (isspace (*s)) s += 1;
	return s;
}

E_ATTR_REPRODUCIBLE
usize
e_cstr_trim_end (const char *s)
{
	if (!s) return 0;
	return e_cstr_trim_end_with_len (s, strlen (s));
}

E_ATTR_REPRODUCIBLE
usize
e_cstr_trim_end_with_len (const char *s, usize len)
{
	if (!s) return 0;
	while (len > 0 && isspace (s[len - 1])) len -= 1;
	return len;
}

E_ATTR_REPRODUCIBLE
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

E_ATTR_REPRODUCIBLE
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

E_Str_Iter
e_cstr_split_init (const char *s)
{
	return (E_Str_Iter) {
		.ptr = s,
		.len = -1, /* -1 required for first iteration to function */
	};
}

bool
e_cstr_split_char (E_Str_Iter *iter, char delim)
{
	const char *delim_pos;

	if (!iter || !iter->ptr || iter->ptr[0] == 0) return false;
	iter->ptr += iter->len + 1;
	delim_pos = e_cstr_find_char (iter->ptr, delim);
	if (delim_pos) {
		iter->len = delim_pos - iter->ptr;
	} else {
		iter->len = strlen (iter->ptr);
	}

	return true;
}

bool
e_cstr_split_str (E_Str_Iter *iter, const char *delim)
{
	const char *delim_pos;

	if (!iter || !iter->ptr || iter->ptr[0] == 0) return false;
	if (!delim || strlen (delim) == 0) {
		iter->ptr += iter->len == (usize) -1 ? 0 : 1;
		iter->len = 1;
		return iter->ptr[0] != 0;
	}

	iter->ptr += iter->len + (iter->len == (usize) -1 ? 1 : strlen (delim));
	delim_pos = e_cstr_find_str (iter->ptr, delim);
	if (delim_pos) {
		iter->len = delim_pos - iter->ptr;
	} else {
		iter->len = strlen (iter->ptr);
	}

	return true;
}

bool
e_cstr_split_lines (E_Str_Iter *iter)
{
	return e_cstr_split_char (iter, '\n');
}

E_ATTR_REPRODUCIBLE
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
E_ATTR_REPRODUCIBLE
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
E_ATTR_REPRODUCIBLE
bool
e_cstr_continues_with (const char *s, const char *expect, usize pos)
{
	if (!s) return false;
	if (!expect) return true;
	if (pos > strlen (s)) return false;
	return e_cstr_eq_n (s + pos, expect, strlen (expect));
}

#endif /* E_CONFIG_MODULE_CSTR */
