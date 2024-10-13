#ifndef _EMPOWER_CSTR_H_
#define _EMPOWER_CSTR_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_CSTR

/*! e_cstr ********************************************************************
 * 
 * This module provides various utility functions for dealing with C strings and
 * serves as an extension to the `string.h` header from the standard library.
 *
 * Module dependencies:
 *  - e_alloc (optional)
 *  - e_debug (transitive; optional)
 *  - e_log (transitive; optional)
 *
 ******************************************************************************/

#include "convey.h"
#include "e_types.h"
#include "e_alloc.h"

/**
 * A pointer to a function that evaulates whether the ASCII character \c matches
 * a certain predicate. It returns 0 if \c is accepted, and non-zero if it is
 * rejected. The functions from `<ctype.h>` have this type.
 */
typedef int (* E_Char_Predicate) (int c);

usize e_cstr_count_char (const char *s, char c);
usize e_cstr_count_char_not (const char *s, char c);
usize e_cstr_count_char_pat (const char *s, const char *accept);
usize e_cstr_count_char_not_pat (const char *s, const char *reject);
usize e_cstr_count_char_func (const char *s, E_Char_Predicate func);
usize e_cstr_count_char_not_func (const char *s, E_Char_Predicate func);
usize e_cstr_count_str (const char *haystack, const char *needle);
usize e_cstr_count_str_overlap (const char *haystack, const char *needle);
usize e_cstr_count_lines (const char *s);
usize e_cstr_len (const char *s);
bool e_cstr_is_ascii (const char *s);
bool e_cstr_is_blank (const char *s);
bool e_cstr_matches_predicate (const char *s, E_Char_Predicate func);
char *e_cstr_to_ascii_lower (char *s);
char *e_cstr_to_ascii_upper (char *s);
char *e_cstr_to_ascii_lower_buf (const char *restrict src, char *restrict dest);
char *e_cstr_to_ascii_upper_buf (const char *restrict src, char *restrict dest);
const char *e_cstr_find_char (const char *s, char c);
const char *e_cstr_find_char_not (const char *s, char c);
const char *e_cstr_find_char_pat (const char *s, const char *accept);
const char *e_cstr_find_char_not_pat (const char *s, const char *reject);
const char *e_cstr_find_char_func (const char *s, E_Char_Predicate func);
const char *e_cstr_find_char_not_func (const char *s, E_Char_Predicate func);
const char *e_cstr_find_str (const char *haystack, const char *needle);
const char *e_cstr_rfind_char (const char *s, char c);
const char *e_cstr_rfind_char_not (const char *s, char c);
const char *e_cstr_rfind_char_pat (const char *s, const char *accept);
const char *e_cstr_rfind_char_not_pat (const char *s, const char *reject);
const char *e_cstr_rfind_char_func (const char *s, E_Char_Predicate func);
const char *e_cstr_rfind_char_not_func (const char *s, E_Char_Predicate func);
const char *e_cstr_rfind_str (const char *haystack, const char *needle);
bool e_cstr_contains (const char *haystack, const char *needle);
bool e_cstr_eq (const char *a, const char *b);
bool e_cstr_eq_n (const char *a, const char *b, usize n);
const char *e_cstr_trim_start (const char *s);
usize e_cstr_trim_end (const char *s);
usize e_cstr_trim_end_with_len (const char *s, usize len);
const char *e_cstr_trim (const char *s, usize *len);
const char *e_cstr_trim_with_len (const char *s, usize *len);
bool e_cstr_starts_with (const char *s, const char *expect);
bool e_cstr_ends_with (const char *s, const char *expect);
bool e_cstr_continues_with (const char *s, const char *expect, usize pos);

#if E_CONFIG_MODULE_ALLOC
usize e_cstr_distance (const char *a, const char *b);
#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_CSTR */
#endif /* _EMPOWER_CSTR_H_ */
