#ifndef _EMPOWER_PARSE_H_
#define _EMPOWER_PARSE_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_PARSE

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
 * Module dependencies:
 *  - e_cstr
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

#if !E_CONFIG_MODULE_CSTR
# error "module e_fs depends on: e_cstr"
#endif /* !E_CONFIG_MODULE_CSTR */

#include "e_compat.h"
#include "e_types.h"
#include "e_cstr.h"

const char *e_parse_char (const char **s, char c);
const char *e_parse_char_not (const char **s, char c);
const char *e_parse_char_pat (const char **s, const char *accept);
const char *e_parse_char_not_pat (const char **s, const char *reject);
const char *e_parse_char_func (const char **s, E_Char_Predicate func);
const char *e_parse_char_not_func (const char **s, E_Char_Predicate func);
const char *e_parse_str_match (const char **s, const char *match);
const char *e_parse_consume_while_char (const char **s, char c);
const char *e_parse_consume_while_pat (const char **s, const char *accept);
const char *e_parse_consume_while_func (const char **s, E_Char_Predicate func);
const char *e_parse_consume_until_char (const char **s, char c);
const char *e_parse_consume_until_pat (const char **s, const char *accept);
const char *e_parse_consume_until_func (const char **s, E_Char_Predicate func);
const char *e_parse_consume_line (const char **s);
const char *e_parse_consume_whitespace (const char **s);
const char *e_parse_consume_whitespace_until_newline (const char **s);

#endif /* E_CONFIG_MODULE_PARSE */
#endif /* _EMPOWER_PARSE_H_ */
