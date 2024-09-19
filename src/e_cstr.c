#include "empower.h"
#include <ctype.h>

#if E_CONFIG_MODULE_CSTR

E_ATTR_PURE
E_ATTR_ACCESS_READ_ONLY (1)
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

E_ATTR_ACCESS_READ_ONLY (1)
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

E_ATTR_PURE
E_ATTR_ACCESS_READ_ONLY (1)
bool
e_cstr_is_ascii (const char *s)
{
	return e_cstr_matches_predicate (s, e_char_is_ascii);
}

E_ATTR_PURE
E_ATTR_ACCESS_READ_ONLY (1)
bool
e_cstr_is_blank (const char *s)
{
	return e_cstr_matches_predicate (s, isspace);
}

E_ATTR_ACCESS_READ_ONLY (1)
bool
e_cstr_matches_predicate (const char *s, e_char_predicate_t func)
{
	if (!s) return true;
	if (!func) return false;
	while (*s) {
		if (!func (*s++)) return false;
	}
	return true;
}

E_ATTR_ACCESS_READ_WRITE (1)
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

E_ATTR_ACCESS_READ_WRITE (1)
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

E_ATTR_ACCESS_READ_ONLY (1)
E_ATTR_ACCESS_WRITE_ONLY (2)
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

E_ATTR_ACCESS_READ_ONLY (1)
E_ATTR_ACCESS_WRITE_ONLY (2)
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

E_ATTR_CONST
int
e_char_is_ascii (int c)
{
	return 0 <= c && c <= 0x7F;
}

#endif /* E_CONFIG_MODULE_CSTR */
