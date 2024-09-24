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
 *  - e_log (transient)
 *
 ******************************************************************************/

E_VEC_DECLARE (char, __E_Vec_Char, __e_vec_char);

/**
 * A dynamically sized string type, internally represented as a \e_vec_t of type
 * `char`. The string data can be accessed through \ptr, and is always
 * terminated with a nul character.
 *
 * @field ptr: The pointer to the string data
 * @field len: The length of the string, excluding the terminating nul character
 * @field cap: The capacity of the string, i.e. the number of items (including
 *             the terminating nul character) it can store befor reallocating.
 *             `cap` is guaranteed to be greater than `len`.
 */
typedef __E_Vec_Char E_Str;

/**
 * Initialise a dynamically sized string with capacity 0. This function does not
 * perform an allocation.
 *
 * @return The new string.
 * @see \e_vec_init
 */
E_Str e_str_init (void);

/**
 * Initialise a dynamically sized string with a given capacity. This function
 * allocates enough memory to store \cap characters (including the terminating
 * nul character).
 *
 * @param cap: The capacity of the string.
 * @return The new string.
 * @see \e_vec_init_with_cap
 */
E_Str e_str_init_with_cap (usize cap);

/**
 * Deinitialise a dynamically sized string and free the memory occupied by it.
 *
 * @param str: The string to deinitialise
 * @see \e_vec_deinit
 */
void e_str_deinit (E_Str *str);

/**
 * Grow the memory of a dynamically sized string \str to at least \cap items
 * (including the terminating nul character). If \cap is lowe than the current
 * capacity of the string, no action will be performed. Internally, it rounds up
 * \cap to the nearest power of two and reallocates the memory to the new
 * capacity.
 *
 * @param str: The string to grow
 * @param cap: The lowe bound for the new capacity
 * @see \e_vec_grow
 */
void e_str_grow (E_Str *str, usize cap);

/**
 * Clone a string. This function allocates memory which must be freed by the
 * user using \e_str_free. If \str is `nullptr`, an string vector is returned.
 *
 * @param str: The string to clone
 * @return A new string
 */
E_Str e_str_clone (E_Str *str);

/**
 * Append a single character \c to the string \str and reallocate if necessary.
 *
 * @param str: The string to change
 * @param c: The character to add
 */
void e_str_append_char (E_Str *str, char c);

/**
 * Append a nul-terminated string \s to the string \str and reallocate if
 * necessary.
 *
 * @param str: The string to change
 * @param s: The nul-terminated string to add
 * @return The length of \s as per `strlen`.
 */
usize e_str_append_cstr (E_Str *str, const char *s);

/**
 * Append a string \s of length \len to the string \str and reallocate if
 * necessary.
 *
 * @param str: The string to change
 * @param s: The string to add
 * @param len: The length of \s
 * @note \s must be at least \len bytes long.
 */
void e_str_append_slice (E_Str *str, const char *s, usize len);

/**
 * Append a formatted string to the string \str and reallocate if necessary.
 * This function works using `vsnprintf`: \fmt serves as the formatting
 * specifier, and only up to \max_fmt_len bytes are actually written to the
 * string.
 *
 * @param str: The string to change
 * @param max_fmt_len: The maximum number of bytes to write
 * @param fmt: The format specifier for `vsnprintf`
 * @param ...: The format arguments for `vsnprintf`
 * @return The number of bytes that would have been written if the entire
 *         formatted string could have been appended. If the entire string was
 *         appended, this is equal to the number of bytes actually written.
 */
[[gnu::format (printf, 3, 4)]]
usize e_str_append_fmt (E_Str *str, usize max_fmt_len, const char *fmt, ...);

/**
 * Append a formatted string to the string \str. The required space is
 * determined automatically and the string is resized accordingly.
 *
 * @param str: The string to change
 * @param fmt: The format specifier for `vsnprintf`
 * @param ...: The format arguments for `vsnprintf`
 * @return The number of bytes that were written to the string.
 */
[[gnu::format (printf, 2, 3)]]
usize e_str_append_fmt_all (E_Str *str, const char *fmt, ...);

#endif /* E_CONFIG_MODULE_STR */
#endif /* _EMPOWER_STR_H_ */
