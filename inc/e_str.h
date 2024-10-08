#ifndef _EMPOWER_STR_H_
#define _EMPOWER_STR_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_STR

#if !E_CONFIG_MODULE_CSTR || !E_CONFIG_MODULE_VEC
# error "module e_str depends on: e_cstr, e_vec"
#endif /* !E_CONFIG_MODULE_VEC */

/*! e_str *********************************************************************
 * 
 * This module provides a dynamically sized string type alongside various string
 * manipulation functions.
 *
 * Module dependencies:
 *  - e_cstr
 *  - e_vec
 *  - e_alloc (transient)
 *  - e_log (transient; optional)
 *
 ******************************************************************************/

E_VEC_DECLARE (char, __E_Vec_Char, __e_vec_char);

/**
 * A dynamically sized string type, internally represented as a `E_Vec` of type
 * `char`. The string data can be accessed through the field \ptr, and is
 * always terminated with a nul character. Length and capacity are stored in the
 * fields \len and \cap, respectively.
 */
typedef __E_Vec_Char E_Str;

/**
 * Initialise a dynamically sized string with capacity 0. This function
 * allocates enough memory to store a string with a nul terminating byte. When
 * the allocation fails, it will terminate the programme with an error message.
 * The user must free the allocated memory with `e_str_free`.
 */
E_Str e_str_init (void);

/**
 * Initialise a dynamically sized string with a given capacity. This function
 * allocates enough memory to store \cap characters (including the terminating
 * nul character). When the allocation fails, it will terminate the programme
 * with an error message. The user must free the allocated memory with
 * `e_str_free`.
 */
E_Str e_str_init_with_cap (usize cap);

/**
 * Deinitialise a dynamically sized string and free the memory occupied by it.
 */
void e_str_deinit (E_Str *str);

/**
 * Grow the memory of a dynamically sized string \str to at least \cap items
 * (including the terminating nul character). If \cap is lower than the current
 * capacity of the string, no action will be performed. Internally, it rounds up
 * \cap to the nearest power of two and reallocates the memory to the new
 * capacity.
 */
void e_str_grow (E_Str *str, usize cap);

/**
 * Clone a string. This function allocates memory which must be freed by the
 * user using \e_str_free. If \str is `nullptr`, an empty string is returned.
 */
E_Str e_str_clone (E_Str *str);

/**
 * Append a single character \c to the string \str and reallocate if necessary.
 */
void e_str_append_char (E_Str *str, char c);

/**
 * Append a nul-terminated string \s to the string \str and reallocate if
 * necessary. Returns `strlen (s)`.
 */
usize e_str_append_cstr (E_Str *str, const char *s);

/**
 * Append a string \s of length \len to the string \str and reallocate if
 * necessary.
 */
void e_str_append_slice (E_Str *str, const char *s, usize len);

/**
 * Append a formatted string to the string \str and reallocate if necessary.
 * This function works using `vsnprintf`: \fmt serves as the formatting
 * specifier, and only up to \max_fmt_len bytes are actually written to the
 * string. Returns the number of bytes that would have been written if the
 * entire formatted string could have been appended. If the entire string was
 * appended, this is equal to the number of bytes actually written.
 */
E_ATTR_FORMAT (printf, 3, 4)
usize e_str_append_fmt (E_Str *str, usize max_fmt_len, const char *fmt, ...);

/**
 * Append a formatted string to the string \str. The required space is
 * determined automatically and the string is resized accordingly. tReturns he
 * number of bytes that were written to the string.
 */
E_ATTR_FORMAT (printf, 2, 3)
usize e_str_append_fmt_all (E_Str *str, const char *fmt, ...);

/**
 * Insert a single character \c at position \idx into the string \str. If \idx
 * is out of bounds, the character is appended instead. When inserting into the
 * middle of the string, the rest of the string is pushed back. The memory is
 * reallocated if necessary.
 */
void e_str_insert_char (E_Str *str, usize idx, char c);

/**
 * Insert a nul-terminated string \s at position \idx into the string \str. If
 * \idx is out of bounds, the character is appended instead. When inserting into
 * the middle of the string, the rest of the string is pushed back. The memory
 * is reallocated if necessary. Retruns the number of characters added to \str,
 * i.e. the length of \s.
 */
usize e_str_insert_cstr (E_Str *str, usize idx, const char *s);

/**
 * Insert a string \s of length \len at position \idx into the string \str. If
 * \idx is out of bounds, the character is appended instead. When inserting into
 * the middle of the string, the rest of the string is pushed back. The memory
 * is reallocated if necessary.
 */
void e_str_insert_slice (E_Str *str, usize idx, const char *s, usize len);

/**
 * Remove \count characters from the string \str, starting at \idx. Returns the
 * number of characters removed. When removing from the middle of the stirng,
 * the memory inside the string is shifted accordingly.
 */
usize e_str_remove (E_Str *str, usize idx, usize count);

#endif /* E_CONFIG_MODULE_STR */
#endif /* _EMPOWER_STR_H_ */
