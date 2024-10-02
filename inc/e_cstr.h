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
usize e_cstr_count_char_matching (const char *s, E_Char_Predicate func);

/**
 * Count the number of occurances of a nul-terminated string \needle in the
 * nul-terminated string \haystack. When \haystack or \needle is `nullptr`, it
 * returns 0.
 */
[[reproducible]]
usize e_cstr_count_str (const char *haystack, const char *needle);

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

/**
 * Find the character \c in the nul-terminated string \s. If no match is found
 * within the string, `nullptr` is returned. Otherwise, a pointer to the matched
 * character is returned. If \s is `nullptr`, `nullptr` is returned. This is
 * essentially like `strchr`, except that it allows for `nullptr` (which would
 * cause undefined behaviour in `strchr`).
 */
[[reproducible]]
const char *e_cstr_find_char (const char *s, char c);

/**
 * Find the first character in the nul-terminated string \s which matches the
 * predicate \func. If no match is found within the string, `nullptr` is
 * returned. Otherwise, a pointer to the matched character is returned. If \s or
 * \func are `nullptr`, `nullptr` is returned.
 */
const char *e_cstr_find_char_matching (const char *s, E_Char_Predicate func);

/**
 * Find the substring \needle in the nul-terminated string \haystack. If no
 * match is found within the string, `nullptr` is returned. Otherwise, a pointer
 * to the matched substring is returned. If \haystack is `nullptr`, `nullptr` is
 * returned. If \needle is `nullptr`, \haystack is returned. This is essentially
 * like `strstr`, except that it allows for `nullptr` (which would cause
 * undefined behaviour in `strstr`).
 */
[[reproducible]]
const char *e_cstr_find_str (const char *haystack, const char *needle);

/**
 * Checks if two strings \a and \b are equal. If both \a and \b are `nullptr`,
 * `true` is returned. If either \a or \b is a `nullptr`, `false` is returned.
 * Otherwise, `strcmp` is used to check if they are equal. This is essentially
 * like using `strcmp() == 0`, except that it allows for `nullptr` (which would
 * cause undefined behaviour in `strcmp`).
 */
bool e_cstr_eq (const char *a, const char *b);

/**
 * Trim whitespace from the start of the nul-terminated string \s. Returns a
 * pointer to the start of the trimmed string. If \s only consists of
 * whitespace, the return value will point to the nul terminator. If `s` is
 * `nullptr`, `nullptr` is returned.
 */
[[reproducible]]
const char *e_cstr_trim_start (const char *s);

/**
 * Trim whitespace at the end of the nul-terminated string \s. Returns the
 * length of the trimmed string. If \s is `nullptr`, 0 is returned.
 */
[[reproducible]]
usize e_cstr_trim_end (const char *s);

/**
 * Trim whitespace at the end of the nul-terminated string \s with a given
 * length \len. \len must be equal to `strlen (s)`. Returns the length of the
 * trimmed string. If \s is `nullptr`, 0 is returned.
 */
[[reproducible]]
usize e_cstr_trim_end_with_len (const char *s, usize len);

/**
 * Trim whitespace at the start and end of the nul-terminated string \s. Returns
 * a pointer to the start of the trimmed string. \len will be set to the length
 * of the trimmed string. If \len is `nullptr`, it will not be filled and this
 * call is essentially equivalent to `e_cstr_trim_start`. If \s is `nullptr`,
 * `nullptr` is returned and \len is set to 0.
 */
[[reproducible]]
const char *e_cstr_trim (const char *s, usize *len);

/**
 * Trim whitespace at the start and end of the nul-terminated string \s of
 * length \len. \len must be equal to `strlen (s)`. Returns a pointer to the
 * start of the trimmed string. \len will be set to the length of the trimmed
 * string. If \len is `nullptr`, it will not be filled and this call is
 * essentially equivalent to `e_cstr_trim_start`. If \s is `nullptr`, `nullptr`
 * is returned and \len is set to 0.
 */
[[reproducible]]
const char *e_cstr_trim_with_len (const char *s, usize *len);

#endif /* E_CONFIG_MODULE_CSTR */
#endif /* _EMPOWER_CSTR_H_ */
