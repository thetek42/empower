#include "e_parse.h"

#if E_CONFIG_MODULE_PARSE

#include <ctype.h>
#include <string.h>

static const char *e_parse_advance (const char **s, usize n);

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched,
 * a pointer to the character (i.e. \s) is returned and \s is advanced. If \c is
 * not matched or \s is `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char (const char **s, char c)
{
	if (!s || !*s || **s == 0) return nullptr;
	if (**s == c) return e_parse_advance (s, 1);
	return nullptr;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched, a pointer to the character (i.e. \s) is returned and \s is advanced.
 * If \c is matched or \s is `nullptr`, `nullptr` is returned and \s is not
 * advanced.
 */
const char *
e_parse_char_not (const char **s, char c)
{
	if (!s || !*s || **s == 0) return nullptr;
	if (**s != c) return e_parse_advance (s, 1);
	return nullptr;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched
 * by (i.e. contained in) the pattern \accept, a pointer to the character (i.e.
 * \s) is returned and \s is advanced. If \c is not matched or either \s or
 * \accept are `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char_pat (const char **s, const char *accept)
{
	if (!s || !*s || **s == 0 || !accept) return nullptr;
	if (strchr (accept, **s) != nullptr) return e_parse_advance (s, 1);
	return nullptr;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched by (i.e. contained in) the pattern \reject or \reject is `nullptr`, a
 * pointer to the character (i.e. \s) is returned and \s is advanced. If \c is
 * matched or \s is `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char_not_pat (const char **s, const char *reject)
{
	if (!s || !*s || **s == 0) return nullptr;
	if (!reject) return *s;
	if (strchr (reject, **s) == nullptr) return e_parse_advance (s, 1);
	return nullptr;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched
 * by the E_Char_Predicate \func, a pointer to the character (i.e. \s) is
 * returned and \s is advanced. If \c is not matched or either \s or \func are
 * `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char_func (const char **s, E_Char_Predicate func)
{
	if (!s || !*s || **s == 0 || !func) return nullptr;
	if (func (**s)) return e_parse_advance (s, 1);
	return nullptr;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched by the E_Char_Predicate \func, a pointer to the character (i.e. \s)
 * is returned and \s is advanced. If \c is matched or either \s or \func are
 * `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char_not_func (const char **s, E_Char_Predicate func)
{
	if (!s || !*s || **s == 0 || !func) return nullptr;
	if (!func (**s)) return e_parse_advance (s, 1);
	return nullptr;
}

/**
 * Parse a nul-terminated string \match in a nul-terminated string \s. If \match
 * the found at the start of \s, \s is advanced to after the match and a pointer
 * to the match (i.e. \s) is returned. If the string is not matched or either \s
 * or \match are `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_str_match (const char **s, const char *match)
{
	usize len;
	if (!s || !*s || **s == 0 || !match) return nullptr;
	len = strlen (match);
	if (strncmp (*s, match, len) == 0) return e_parse_advance (s, len);
	return nullptr;
}

/**
 * Consume a nul-terminated string \s while a character \c is matched and
 * advance \s to the first character which is not matched. Returns \s.
 */
const char *
e_parse_consume_while_char (const char **s, char c)
{
	const char *r;
	if (!s || !*s) return nullptr;
	r = *s;
	while (**s && **s == c) *s += 1;
	return r;
}

/**
 * Consume a nul-terminated string \s while a character is matched by (i.e.
 * contained in) the pattern \accept and advance \s to the first character which
 * is not matched. Returns \s.
 */
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

/**
 * Consume a nul-terminated string \s while a character is matched by the
 * E_Char_Predicate \func and advance \s to the first character which is not
 * matched. Returns \s.
 */
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

/**
 * Consume a nul-terminated string \s until a character \c is matched and
 * advance \s to the first character which is matched. Returns \s.
 */
const char *
e_parse_consume_until_char (const char **s, char c)
{
	const char *r;
	if (!s || !*s) return nullptr;
	r = *s;
	while (**s && **s != c) *s += 1;
	return r;
}

/**
 * Consume a nul-terminated string \s until a character is matched by (i.e.
 * contained in) the pattern \accept and advance \s to the first character which
 * is matched. Returns \s.
 */
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

/**
 * Consume a nul-terminated string \s until a character is matched by the
 * E_Char_Predicate \func and advance \s to the first character which is
 * matched. Returns \s.
 */
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

/**
 * Consume a nul-terminated string \s until the end of the line and advance \s
 * to the newline character or end of string. Returns \s. Note that the newline
 * character is not consumed and needs to be checked.
 */
const char *
e_parse_consume_line (const char **s)
{
	return e_parse_consume_until_char (s, '\n');
}

/**
 * Consume whitespace in a nul-terminated string \s and advance \s to the first
 * non-whitespace character. Returns \s.
 */
const char *
e_parse_consume_whitespace (const char **s)
{
	return e_parse_consume_while_func (s, isspace);
}


/**
 * Consume whitespace in a nul-terminated string \s and advance \s to the first
 * non-whitespace or newline character. Returns \s. Note that the newline
 * character is not consumed and needs to be checked:
 *
 *  | e_parse_consume_whitespace_until_newline (&s);
 *  | if (!e_parse_char (&s, '\n')) return FAIL ("unexpected trailing chars");
 */
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
