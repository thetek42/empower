#include "empower.h"
#include <ctype.h>

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
e_cstr_count_func (const char *s, e_char_predicate_t func)
{
	usize r;

	r = 0;
	if (!s || !func) return 0;
	while (*s) {
		if (func (*s++)) r += 1;
	}
	return r;
}

bool
e_cstr_is_blank (const char *s)
{
	return e_cstr_matches_predicate (s, isspace);
}

bool
e_cstr_is_ascii (const char *s)
{
	return e_cstr_matches_predicate (s, e_char_is_ascii);
}

bool
e_cstr_matches_predicate (const char *s, e_char_predicate_t func)
{
	if (!s || !func) return true;
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
	while (src && dest && *src) {
		*dest = isupper (*src) ? *src + 32 : *src;
		src++;
		dest++;
	}
	return dest;
}

char *
e_cstr_to_ascii_upper_buf (const char *restrict src, char *restrict dest)
{
	while (src && dest && *src) {
		*dest = islower (*src) ? *src - 32 : *src;
		src++;
		dest++;
	}
	return dest;
}

int
e_char_is_ascii (int c)
{
	return 0 <= c && c <= 0x7F;
}
