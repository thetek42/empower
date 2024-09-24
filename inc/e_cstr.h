#ifndef _EMPOWER_CSTR_H_
#define _EMPOWER_CSTR_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_CSTR

/*! e_cstr ********************************************************************
 * 
 * This module provides various utility functions for dealing with C strings and
 * serves as an extension to the `string.h` header from the standard library.
 *
 ******************************************************************************/

/**
 * A pointer to a function that evaulates whether the ASCII character \c matches
 * a certain predicate. It returns 0 if \c does not match, and non-zero if it
 * does. The functions from `<ctype.h>` have this type.
 */
typedef int (* E_Char_Predicate) (int c);

/**
 * Count the number of occurances of a character \c in the nul-terminated string
 * \s. When \s is `nullptr`, it returns 0.
 */
[[reproducible]]
usize e_cstr_count_char (const char *s, char c);

/**
 * Count the number of matches of a `E_Char_Predicate` \func in the
 * nul-terminated string \s. When \s or \func are `nullptr`, it returns 0.
 */
usize e_cstr_count_func (const char *s, E_Char_Predicate func);

/**
 * Check if the nul-terminated string \s is ASCII. Returns `true` when \s only
 * consists of ASCII characters or if \s is `nullptr`.
 */
[[reproducible]]
bool e_cstr_is_ascii (const char *s);

/**
 * Check if the nul-terminated string \s only consists of whitespace characters
 * as per `isspace`. Returns `true` when \s only consists of whitespace or if \s
 * is `nullptr`.
 */
[[reproducible]]
bool e_cstr_is_blank (const char *s);

/**
 * Check if the nul-terminated string \s matches the `E_Char_Predicate` \func.
 * Returns `true` when \s only consists of characters that were accepted by
 * \func or if \s is `nullptr`, and returns false if \func is `nullptr` or did
 * not match.
 */
bool e_cstr_matches_predicate (const char *s, E_Char_Predicate func);

/**
 * Convert uppercase ASCII characters the nul-terminated string \s to lowercase
 * in-place. When \s is a `nullptr`, no action is performed. Returns \s.
 */
char *e_cstr_to_ascii_lower (char *s);

/**
 * Convert lowercase ASCII characters the nul-terminated string \s to uppercase
 * in-place. When \s is a `nullptr`, no action is performed. Returns \s.
 */
char *e_cstr_to_ascii_upper (char *s);

/**
 * Convert uppercase ASCII characters the nul-terminated string \src to
 * lowercase and store them in \dest. The terminating nul byte in \dest is
 * written. When \src or \dest are `nullptr`s, no action is performed. \dest
 * must have enough space to store all of \src, including the terminating nul
 * byte.
 */
char *e_cstr_to_ascii_lower_buf (const char *restrict src, char *restrict dest);

/**
 * Convert uppercase ASCII characters the nul-terminated string \src to
 * lowercase and store them in \dest. The terminating nul byte in \dest is
 * written. When \src or \dest are `nullptr`s, no action is performed. \dest
 * must have enough space to store all of \src, including the terminating nul
 * byte.
 */
char *e_cstr_to_ascii_upper_buf (const char *restrict src, char *restrict dest);

#endif /* E_CONFIG_MODULE_CSTR */
#endif /* _EMPOWER_CSTR_H_ */
