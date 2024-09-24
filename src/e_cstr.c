#include "empower.h"

#if E_CONFIG_MODULE_CSTR

[[reproducible]]
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

usize
e_cstr_count_func (const char *s, E_Char_Predicate func)
{
	usize r;

	r = 0;
	if (!s || !func) return 0;
	while (*s) {
		if (func (*s++)) r += 1;
	}
	return r;
}

[[reproducible]]
bool
e_cstr_is_ascii (const char *s)
{
	return e_cstr_matches_predicate (s, isascii);
}

[[reproducible]]
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

const char *
e_cstr_find_matching (const char *s, E_Char_Predicate func)
{
	if (!s || !func) return s;

	while (*s) {
		if (func (*s)) return s;
		s++;
	}

	return nullptr;
}

bool
e_cstr_eq (const char *a, const char *b)
{
	if ((!a && b) || (a && !b)) return false;
	if (!a && !b) return true;
	return !strcmp (a, b);
}

#endif /* E_CONFIG_MODULE_CSTR */
