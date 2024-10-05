#ifndef _EMPOWER_PARSE_H_
#define _EMPOWER_PARSE_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_PARSE

#if !E_CONFIG_MODULE_CSTR
# error "module e_fs depends on: e_cstr"
#endif /* !E_CONFIG_MODULE_CSTR */

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
 * | const char *input = "...";
 * | const char *s = input;
 * | const char *name;
 * | usize name_len;
 * |
 * | if (!e_parse_char (&s, '[')) return FAIL ("expected `[`, found `%c`", *s);
 * | name = e_parse_consume_while_func (&s, isalnum));
 * | name_len = s - name;
 * | if (!e_parse_char (&s, ']')) return FAIL ("expected `]`, found `%c`", *s);
 * | if (*s != '\0') return FAIL ("unexpected trailing chars");
 * |
 * | printf ("name: \"%s\" (len %zu)\n", name, name_len);
 *
 * Module dependencies:
 *  - e_cstr
 *
 ******************************************************************************/

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched,
 * a pointer to the character (i.e. \s) is returned and \s is advanced. If \c is
 * not matched or \s is `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *e_parse_char (const char **s, char c);

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched, a pointer to the character (i.e. \s) is returned and \s is advanced.
 * If \c is matched or \s is `nullptr`, `nullptr` is returned and \s is not
 * advanced.
 */
const char *e_parse_char_not (const char **s, char c);

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched
 * by (i.e. contained in) the pattern \accept, a pointer to the character (i.e.
 * \s) is returned and \s is advanced. If \c is not matched or either \s or
 * \accept are `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *e_parse_char_pat (const char **s, const char *accept);

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched by (i.e. contained in) the pattern \reject or \reject is `nullptr`, a
 * pointer to the character (i.e. \s) is returned and \s is advanced. If \c is
 * matched or \s is `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *e_parse_char_not_pat (const char **s, const char *reject);

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched
 * by the E_Char_Predicate \func, a pointer to the character (i.e. \s) is
 * returned and \s is advanced. If \c is not matched or either \s or \func are
 * `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *e_parse_char_func (const char **s, E_Char_Predicate func);

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched by the E_Char_Predicate \func, a pointer to the character (i.e. \s)
 * is returned and \s is advanced. If \c is matched or either \s or \func are
 * `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *e_parse_char_not_func (const char **s, E_Char_Predicate func);

/**
 * Parse a nul-terminated string \match in a nul-terminated string \s. If \match
 * the found at the start of \s, \s is advanced to after the match and a pointer
 * to the match (i.e. \s) is returned. If the string is not matched or either \s
 * or \match are `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *e_parse_str_match (const char **s, const char *match);

/**
 * Consume a nul-terminated string \s while a character \c is matched and
 * advance \s to the first character which is not matched. Returns \s.
 */
const char *e_parse_consume_while_char (const char **s, char c);

/**
 * Consume a nul-terminated string \s while a character is matched by (i.e.
 * contained in) the pattern \accept and advance \s to the first character which
 * is not matched. Returns \s.
 */
const char *e_parse_consume_while_pat (const char **s, const char *accept);

/**
 * Consume a nul-terminated string \s while a character is matched by the
 * E_Char_Predicate \func and advance \s to the first character which is not
 * matched. Returns \s.
 */
const char *e_parse_consume_while_func (const char **s, E_Char_Predicate func);

/**
 * Consume a nul-terminated string \s until a character \c is matched and
 * advance \s to the first character which is matched. Returns \s.
 */
const char *e_parse_consume_until_char (const char **s, char c);

/**
 * Consume a nul-terminated string \s until a character is matched by (i.e.
 * contained in) the pattern \accept and advance \s to the first character which
 * is matched. Returns \s.
 */
const char *e_parse_consume_until_pat (const char **s, const char *accept);

/**
 * Consume a nul-terminated string \s until a character is matched by the
 * E_Char_Predicate \func and advance \s to the first character which is
 * matched. Returns \s.
 */
const char *e_parse_consume_until_func (const char **s, E_Char_Predicate func);

/**
 * Consume a nul-terminated string \s until the end of the line and advance \s
 * to the newline character or end of string. Returns \s. Note that the newline
 * character is not consumed and needs to be checked.
 */
const char *e_parse_consume_line (const char **s);

/**
 * Consume whitespace in a nul-terminated string \s and advance \s to the first
 * non-whitespace character. Returns \s.
 */
const char *e_parse_consume_whitespace (const char **s);

/**
 * Consume whitespace in a nul-terminated string \s and advance \s to the first
 * non-whitespace or newline character. Returns \s. Note that the newline
 * character is not consumed and needs to be checked:
 *
 * | e_parse_consume_whitespace_until_newline (&s);
 * | if (!e_parse_char (&s, '\n')) return FAIL ("unexpected trailing chars");
 */
const char *e_parse_consume_whitespace_until_newline (const char **s);

#endif /* E_CONFIG_MODULE_PARSE */
#endif /* _EMPOWER_PARSE_H_ */
