#include "empower.h"

#if E_CONFIG_MODULE_PARSE

static const char *e_parse_advance (const char **s, usize n);

const char *
e_parse_char (const char **s, char c)
{
	if (!s || !*s || **s == 0) return nullptr;
	if (**s == c) return e_parse_advance (s, 1);
	return nullptr;
}

const char *
e_parse_char_not (const char **s, char c)
{
	if (!s || !*s || **s == 0) return nullptr;
	if (**s != c) return e_parse_advance (s, 1);
	return nullptr;
}

const char *
e_parse_char_pat (const char **s, const char *accept)
{
	if (!s || !*s || **s == 0 || !accept) return nullptr;
	if (strchr (accept, **s) != nullptr) return e_parse_advance (s, 1);
	return nullptr;
}

const char *
e_parse_char_not_pat (const char **s, const char *reject)
{
	if (!s || !*s || **s == 0) return nullptr;
	if (!reject) return *s;
	if (strchr (reject, **s) == nullptr) return e_parse_advance (s, 1);
	return nullptr;
}

const char *
e_parse_char_func (const char **s, E_Char_Predicate func)
{
	if (!s || !*s || **s == 0 || !func) return nullptr;
	if (func (**s)) return e_parse_advance (s, 1);
	return nullptr;
}

const char *
e_parse_char_not_func (const char **s, E_Char_Predicate func)
{
	if (!s || !*s || **s == 0 || !func) return nullptr;
	if (!func (**s)) return e_parse_advance (s, 1);
	return nullptr;
}

const char *
e_parse_str_match (const char **s, const char *match)
{
	usize len;
	if (!s || !*s || **s == 0 || !match) return nullptr;
	len = strlen (match);
	if (strncmp (*s, match, len) == 0) return e_parse_advance (s, len);
	return nullptr;
}

const char *
e_parse_consume_while_char (const char **s, char c)
{
	const char *r;
	if (!s || !*s) return nullptr;
	r = *s;
	while (**s && **s == c) *s += 1;
	return r;
}

const char *
e_parse_consume_while_pat (const char **s, const char *accept)
{
	const char *r;
	if (!s || !*s) return nullptr;
	if (!accept) return *s;
	r = *s;
	while (**s && strchr (accept, **s) != nullptr) *s += 1;
	return r;
}

const char *
e_parse_consume_while_func (const char **s, E_Char_Predicate func)
{
	const char *r;
	if (!s || !*s) return nullptr;
	if (!func) return *s;
	r = *s;
	while (**s && func (**s)) *s += 1;
	return r;
}

const char *
e_parse_consume_until_char (const char **s, char c)
{
	const char *r;
	if (!s || !*s) return nullptr;
	r = *s;
	while (**s && **s != c) *s += 1;
	return r;
}

const char *
e_parse_consume_until_pat (const char **s, const char *accept)
{
	const char *r;
	if (!s || !*s) return nullptr;
	if (!accept) return *s;
	r = *s;
	while (**s && strchr (accept, **s) == nullptr) *s += 1;
	return r;
}

const char *
e_parse_consume_until_func (const char **s, E_Char_Predicate func)
{
	const char *r;
	if (!s || !*s) return nullptr;
	if (!func) return *s;
	r = *s;
	while (**s && !func (**s)) *s += 1;
	return r;
}

const char *
e_parse_consume_line (const char **s)
{
	return e_parse_consume_until_char (s, '\n');
}

const char *
e_parse_consume_whitespace (const char **s)
{
	return e_parse_consume_while_func (s, isspace);
}

const char *
e_parse_consume_whitespace_until_newline (const char **s)
{
	return e_parse_consume_while_pat (s, " \f\t\v\r");
}

static const char *
e_parse_advance (const char **s, usize n)
{
	const char *r;
	r = *s;
	*s += n;
	return r;
}

#endif /* E_CONFIG_MODULE_PARSE */
