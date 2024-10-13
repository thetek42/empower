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

E_Str e_str_init (void);
E_Str e_str_init_with_cap (usize cap);
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

#endif /* E_CONFIG_MODULE_STR */
#endif /* _EMPOWER_STR_H_ */
