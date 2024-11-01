#ifndef _EMPOWER_PARSE_H_
#define _EMPOWER_PARSE_H_

/*! e_parse *******************************************************************
 *
 * This module provides utilities for writing parsers.
 *
 * The functions below operate on a `const char **`, meaning that after parsing
 * succeeded, the pointer will be advanced automatically to after the end of the
 * previously parsed token. Either the result of the parsing or the location of
 * the token start is returned. If no match could be returned, `nullptr` is
 * used. Refer to the documentation of the individual functions for their
 * precise behaviour.
 *
 * The following example illustrates how to parse a string such as "[foo]",
 * where "foo" can be any sequence of uppercase or lowercase ASCII characters.
 *
 *  | const char *input = "...";
 *  | const char *s = input;
 *  | const char *name;
 *  | usize name_len;
 *  |
 *  | if (!e_parse_char (&s, '[')) return FAIL ("expected `[`, found `%c`", *s);
 *  | name = e_parse_consume_while_func (&s, isalnum));
 *  | name_len = s - name;
 *  | if (!e_parse_char (&s, ']')) return FAIL ("expected `]`, found `%c`", *s);
 *  | if (*s != '\0') return FAIL ("unexpected trailing chars");
 *  |
 *  | printf ("name: \"%s\" (len %zu)\n", name, name_len);
 *
 ******************************************************************************/

/* public interface ***********************************************************/

/**
 * A pointer to a function that evaulates whether the ASCII character \c matches
 * a certain predicate. It returns 0 if \c is accepted, and non-zero if it is
 * rejected. The functions from `<ctype.h>` have this type.
 */
typedef int (* E_Parse_Char_Predicate) (int c);

const char *e_parse_char (const char **s, char c);
const char *e_parse_char_not (const char **s, char c);
const char *e_parse_char_pat (const char **s, const char *accept);
const char *e_parse_char_not_pat (const char **s, const char *reject);
const char *e_parse_char_func (const char **s, E_Parse_Char_Predicate func);
const char *e_parse_char_not_func (const char **s, E_Parse_Char_Predicate func);
const char *e_parse_str_match (const char **s, const char *match);
const char *e_parse_consume_while_char (const char **s, char c);
const char *e_parse_consume_while_pat (const char **s, const char *accept);
const char *e_parse_consume_while_func (const char **s, E_Parse_Char_Predicate func);
const char *e_parse_consume_until_char (const char **s, char c);
const char *e_parse_consume_until_pat (const char **s, const char *accept);
const char *e_parse_consume_until_func (const char **s, E_Parse_Char_Predicate func);
const char *e_parse_consume_line (const char **s);
const char *e_parse_consume_whitespace (const char **s);
const char *e_parse_consume_whitespace_until_newline (const char **s);

/* implementation *************************************************************/

#ifdef E_PARSE_IMPL

#include <ctype.h>
#include <stddef.h>
#include <string.h>

static const char *e_parse_priv_advance (const char **s, size_t n);

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched,
 * a pointer to the character (i.e. \s) is returned and \s is advanced. If \c is
 * not matched or \s is `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char (const char **s, char c)
{
	if (!s || !*s || **s == 0) return NULL;
	if (**s == c) return e_parse_priv_advance (s, 1);
	return NULL;
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
	if (!s || !*s || **s == 0) return NULL;
	if (**s != c) return e_parse_priv_advance (s, 1);
	return NULL;
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
	if (!s || !*s || **s == 0 || !accept) return NULL;
	if (strchr (accept, **s) != NULL) return e_parse_priv_advance (s, 1);
	return NULL;
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
	if (!s || !*s || **s == 0) return NULL;
	if (!reject) return *s;
	if (strchr (reject, **s) == NULL) return e_parse_priv_advance (s, 1);
	return NULL;
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
	if (!s || !*s || **s == 0 || !func) return NULL;
	if (func (**s)) return e_parse_priv_advance (s, 1);
	return NULL;
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
	if (!s || !*s || **s == 0 || !func) return NULL;
	if (!func (**s)) return e_parse_priv_advance (s, 1);
	return NULL;
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
	size_t len;
	if (!s || !*s || **s == 0 || !match) return NULL;
	len = strlen (match);
	if (strncmp (*s, match, len) == 0) return e_parse_priv_advance (s, len);
	return NULL;
}

/**
 * Consume a nul-terminated string \s while a character \c is matched and
 * advance \s to the first character which is not matched. Returns \s.
 */
const char *
e_parse_consume_while_char (const char **s, char c)
{
	const char *r;
	if (!s || !*s) return NULL;
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
	if (!s || !*s) return NULL;
	if (!accept) return *s;
	r = *s;
	while (**s && strchr (accept, **s) != NULL) *s += 1;
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
	if (!s || !*s) return NULL;
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
	if (!s || !*s) return NULL;
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
	if (!s || !*s) return NULL;
	if (!accept) return *s;
	r = *s;
	while (**s && strchr (accept, **s) == NULL) *s += 1;
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
	if (!s || !*s) return NULL;
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
e_parse_priv_advance (const char **s, size_t n)
{
	const char *r;
	r = *s;
	*s += n;
	return r;
}

#endif /* E_PARSE_IMPL */

#endif /* _EMPOWER_PARSE_H_ */
