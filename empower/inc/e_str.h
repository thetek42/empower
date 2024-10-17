#ifndef _EMPOWER_STR_H_
#define _EMPOWER_STR_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_STR

/*! e_str *********************************************************************
 * 
 * This module provides a dynamically sized string type alongside various string
 * manipulation functions.
 *
 * Module dependencies:
 *  - e_cstr
 *  - e_vec
 *  - e_alloc (transient)
 *  - e_debug (transient)
 *  - e_log (transient; optional)
 *
 ******************************************************************************/

#if !E_CONFIG_MODULE_CSTR || !E_CONFIG_MODULE_VEC
# error "module e_str depends on: e_cstr, e_vec"
#endif /* !E_CONFIG_MODULE_VEC */

#include "convey.h"
#include "e_types.h"
#include "e_cstr.h"
#include "e_vec.h"

E_VEC_DECLARE (char, __E_Vec_Char, __e_vec_char);

/**
 * A dynamically sized string type, internally represented as a `E_Vec` of type
 * `char`. The string data can be accessed through the field \ptr, and is
 * always terminated with a nul character. Length and capacity are stored in the
 * fields \len and \cap, respectively.
 */
typedef __E_Vec_Char E_Str;

/* dynamic string functions ***************************************************/

E_Str e_str_init (void);
E_Str e_str_init_with_cap (usize cap);
E_Str e_str_from_cstr (const char *s);
E_Str e_str_from_slice (const char *s, usize len);
E_Str e_str_from_allocated (char *ptr, usize len, usize cap);
void e_str_deinit (E_Str *str);
void e_str_grow (E_Str *str, usize cap);
E_Str e_str_clone (E_Str *str);
void e_str_append_char (E_Str *str, char c);
usize e_str_append_cstr (E_Str *str, const char *s);
void e_str_append_slice (E_Str *str, const char *s, usize len);
C_ATTR_FORMAT (printf, 3, 4) usize e_str_append_fmt (E_Str *str, usize max_fmt_len, const char *fmt, ...);
C_ATTR_FORMAT (printf, 2, 3) usize e_str_append_fmt_all (E_Str *str, const char *fmt, ...);
void e_str_insert_char (E_Str *str, usize idx, char c);
usize e_str_insert_cstr (E_Str *str, usize idx, const char *s);
void e_str_insert_slice (E_Str *str, usize idx, const char *s, usize len);
usize e_str_remove (E_Str *str, usize idx, usize count);
void e_str_trim_start (E_Str *str);
void e_str_trim_end (E_Str *str);
void e_str_trim (E_Str *str);

/* e_cstr wrappers ************************************************************/

usize e_str_count_char (const E_Str *s, char c);
usize e_str_count_char_not (const E_Str *s, char c);
usize e_str_count_char_pat (const E_Str *s, const char *accept);
usize e_str_count_char_not_pat (const E_Str *s, const char *reject);
usize e_str_count_char_func (const E_Str *s, E_Char_Predicate func);
usize e_str_count_char_not_func (const E_Str *s, E_Char_Predicate func);
usize e_str_count_str (const E_Str *haystack, const char *needle);
usize e_str_count_str_overlap (const E_Str *haystack, const char *needle);
usize e_str_count_lines (const E_Str *s);
bool e_str_is_ascii (const E_Str *s);
bool e_str_is_blank (const E_Str *s);
bool e_str_matches_predicate (const E_Str *s, E_Char_Predicate func);
void e_str_to_ascii_lower (E_Str *s);
void e_str_to_ascii_upper (E_Str *s);
const char *e_str_find_char (const E_Str *s, char c);
const char *e_str_find_char_not (const E_Str *s, char c);
const char *e_str_find_char_pat (const E_Str *s, const char *accept);
const char *e_str_find_char_not_pat (const E_Str *s, const char *reject);
const char *e_str_find_char_func (const E_Str *s, E_Char_Predicate func);
const char *e_str_find_char_not_func (const E_Str *s, E_Char_Predicate func);
const char *e_str_find_str (const E_Str *haystack, const char *needle);
const char *e_str_rfind_char (const E_Str *s, char c);
const char *e_str_rfind_char_not (const E_Str *s, char c);
const char *e_str_rfind_char_pat (const E_Str *s, const char *accept);
const char *e_str_rfind_char_not_pat (const E_Str *s, const char *reject);
const char *e_str_rfind_char_func (const E_Str *s, E_Char_Predicate func);
const char *e_str_rfind_char_not_func (const E_Str *s, E_Char_Predicate func);
const char *e_str_rfind_str (const E_Str *haystack, const char *needle);
bool e_str_contains (const E_Str *haystack, const char *needle);
bool e_str_eq (const E_Str *a, const char *b);
bool e_str_eq_n (const E_Str *a, const char *b, usize n);
bool e_str_starts_with (const E_Str *s, const char *expect);
bool e_str_ends_with (const E_Str *s, const char *expect);
bool e_str_continues_with (const E_Str *s, const char *expect, usize pos);
usize e_str_distance (const E_Str *a, const char *b);

#endif /* E_CONFIG_MODULE_STR */
#endif /* _EMPOWER_STR_H_ */
