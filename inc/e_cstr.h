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

/* TODO: allocating string splitters */

/**
 * A struct for use with string iteration functions (such as the `e_cstr_split`
 * family of functions). This is essentially just a memory slice where \ptr
 * points to the beginning of the substring, and \len informs the user about how
 * long the substring is. See `e_cstr_split_char` for how to use this.
 */
typedef struct {
	const char *ptr;
	usize len;
} E_Str_Iter;

/**
 * A pointer to a function that evaulates whether the ASCII character \c matches
 * a certain predicate. It returns 0 if \c is accepted, and non-zero if it is
 * rejected. The functions from `<ctype.h>` have this type.
 */
typedef int (* E_Char_Predicate) (int c);

/**
 * Count the number of occurances of a character \c in the nul-terminated string
 * \s. When \s is `nullptr`, it returns 0.
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_count_char (const char *s, char c);

/**
 * Count the number of occurances of characters which are not \c in the
 * nul-terminated string \s. When \s is `nullptr`, it returns 0.
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_count_char_not (const char *s, char c);

/**
 * Count the number of occurances of characters in the nul-terminated string \s
 * which are contained in the nul-terminated string \accept. When \s or \accept
 * is `nullptr`, it returns 0.
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_count_char_pat (const char *s, const char *accept);

/**
 * Count the number of occurances of characters in the nul-terminated string \s
 * which are not contained in the nul-terminated string \reject. When \s is
 * `nullptr`, it returns 0. When \reject is `nullptr`, it returns `strlen (s)`.
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_count_char_not_pat (const char *s, const char *reject);

/**
 * Count the number of occurances of characters in the nul-terminated string \s
 * which are accepted by the `E_Char_Predicate` \func. When \s or \func are
 * `nullptr`, it returns 0.
 */
usize e_cstr_count_char_func (const char *s, E_Char_Predicate func);

/**
 * Count the number of occurances of characters in the nul-terminated string \s
 * which are rejected by the `E_Char_Predicate` \func. When \s or \func are
 * `nullptr`, it returns 0.
 */
usize e_cstr_count_char_not_func (const char *s, E_Char_Predicate func);

/**
 * Count the number of occurances of a nul-terminated string \needle in the
 * nul-terminated string \haystack. When \haystack or \needle is `nullptr`, it
 * returns 0.
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_count_str (const char *haystack, const char *needle);

/**
 * Count the number of lines in the nul-terminated string \s. When \s is
 * `nullptr`, it returns 0.
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_count_lines (const char *s);

/**
 * Get the length of a string \s. If \s is `nullptr`, 0 is returned. This is
 * essentially like `strlen`, except that it allows for `nullptr` (which would
 * cause undefined behaviour in `strlen`)
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_len (const char *s);

/**
 * Check if the nul-terminated string \s is ASCII. Returns `true` when \s only
 * consists of ASCII characters or if \s is `nullptr`.
 */
E_ATTR_REPRODUCIBLE
bool e_cstr_is_ascii (const char *s);

/**
 * Check if the nul-terminated string \s only consists of whitespace characters
 * as per `isspace`. Returns `true` when \s only consists of whitespace or if \s
 * is `nullptr`.
 */
E_ATTR_REPRODUCIBLE
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
E_ATTR_REPRODUCIBLE
const char *e_cstr_find_char (const char *s, char c);

/**
 * Find the first character which is not \c in the nul-terminated string \s. If
 * no match is found within the string, `nullptr` is returned. Otherwise, a
 * pointer to the matched character is returned. If \s is `nullptr`, `nullptr`
 * is returned.
 */
E_ATTR_REPRODUCIBLE
const char *e_cstr_find_char_not (const char *s, char c);

/**
 * Find the first character in the nul-terminated string \s which is also
 * contained in the nul-terminated string \accept. If no match is found within
 * the string, `nullptr` is returned. Otherwise, a pointer to the matched
 * character is returned. If \s or \accept are `nullptr`, `nullptr` is returned.
 */
E_ATTR_REPRODUCIBLE
const char *e_cstr_find_char_pat (const char *s, const char *accept);

/**
 * Find the first character in the nul-terminated string \s which is not
 * contained in the nul-terminated string \reject. If no match is found within
 * the string, `nullptr` is returned. Otherwise, a pointer to the matched
 * character is returned. If \s or \reject are `nullptr`, \s is returned.
 */
E_ATTR_REPRODUCIBLE
const char *e_cstr_find_char_not_pat (const char *s, const char *reject);

/**
 * Find the first character in the nul-terminated string \s which matches the
 * predicate \func. If no match is found within the string, `nullptr` is
 * returned. Otherwise, a pointer to the matched character is returned. If \s or
 * \func are `nullptr`, `nullptr` is returned.
 */
const char *e_cstr_find_char_func (const char *s, E_Char_Predicate func);

/**
 * Find the first character in the nul-terminated string \s which is rejected by
 * the predicate \func. If no match is found within the string, `nullptr` is
 * returned. Otherwise, a pointer to the matched character is returned. If \s is
 * `nullptr`, `nullptr` is returned. If \func is `nullptr`, \s is returned.
 */
const char *e_cstr_find_char_not_func (const char *s, E_Char_Predicate func);

/**
 * Find the substring \needle in the nul-terminated string \haystack. If no
 * match is found within the string, `nullptr` is returned. Otherwise, a pointer
 * to the matched substring is returned. If \haystack is `nullptr`, `nullptr` is
 * returned. If \needle is `nullptr`, \haystack is returned. This is essentially
 * like `strstr`, except that it allows for `nullptr` (which would cause
 * undefined behaviour in `strstr`).
 */
E_ATTR_REPRODUCIBLE
const char *e_cstr_find_str (const char *haystack, const char *needle);

/**
 * Check if the nul-terminated string \haystack contains the nul-terminated
 * string \needle. If \haystack is `nullptr`, `false` is returned. If \needle is
 * `nullptr`, `true` is returned.
 */
E_ATTR_REPRODUCIBLE
bool e_cstr_contains (const char *haystack, const char *needle);

/**
 * Checks if two strings \a and \b are equal. If both \a and \b are `nullptr`,
 * `true` is returned. If either \a or \b is a `nullptr`, `false` is returned.
 * Otherwise, `strcmp` is used to check if they are equal. This is essentially
 * like using `strcmp() == 0`, except that it allows for `nullptr` (which would
 * cause undefined behaviour in `strcmp`).
 */
E_ATTR_REPRODUCIBLE
bool e_cstr_eq (const char *a, const char *b);

/**
 * Checks if two strings \a and \b are equal in the first \n bytes. If both \a
 * and \b are `nullptr`, `true` is returned. If either \a or \b is a `nullptr`,
 * `false` is returned. Otherwise, `strncmp` is used to check if they are equal.
 * This is essentially like using `strncmp() == 0`, except that it allows for
 * `nullptr` (which would cause undefined behaviour in `strncmp`).
 */
E_ATTR_REPRODUCIBLE
bool e_cstr_eq_n (const char *a, const char *b, usize n);

/**
 * Trim whitespace from the start of the nul-terminated string \s. Returns a
 * pointer to the start of the trimmed string. If \s only consists of
 * whitespace, the return value will point to the nul terminator. If `s` is
 * `nullptr`, `nullptr` is returned.
 */
E_ATTR_REPRODUCIBLE
const char *e_cstr_trim_start (const char *s);

/**
 * Trim whitespace at the end of the nul-terminated string \s. Returns the
 * length of the trimmed string. If \s is `nullptr`, 0 is returned.
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_trim_end (const char *s);

/**
 * Trim whitespace at the end of the nul-terminated string \s with a given
 * length \len. \len must be equal to `strlen (s)`. Returns the length of the
 * trimmed string. If \s is `nullptr`, 0 is returned.
 */
E_ATTR_REPRODUCIBLE
usize e_cstr_trim_end_with_len (const char *s, usize len);

/**
 * Trim whitespace at the start and end of the nul-terminated string \s. Returns
 * a pointer to the start of the trimmed string. \len will be set to the length
 * of the trimmed string. If \len is `nullptr`, it will not be filled and this
 * call is essentially equivalent to `e_cstr_trim_start`. If \s is `nullptr`,
 * `nullptr` is returned and \len is set to 0.
 */
E_ATTR_REPRODUCIBLE
const char *e_cstr_trim (const char *s, usize *len);

/**
 * Trim whitespace at the start and end of the nul-terminated string \s of
 * length \len. \len must be equal to `strlen (s)`. Returns a pointer to the
 * start of the trimmed string. \len will be set to the length of the trimmed
 * string. If \len is `nullptr`, it will not be filled and this call is
 * essentially equivalent to `e_cstr_trim_start`. If \s is `nullptr`, `nullptr`
 * is returned and \len is set to 0.
 */
E_ATTR_REPRODUCIBLE
const char *e_cstr_trim_with_len (const char *s, usize *len);

/**
 * Initialize a `E_Str_Iter` for the `e_cstr_split` family of functions with the
 * nul-terminated string \s.
 */
E_Str_Iter e_cstr_split_init (const char *s);

/**
 * Iterate on a string iterator \iter by splitting the string at \delim. If
 * there is a segment that can be processed, it returns `true`, or `false` if
 * there is no new segment. \iter is updated to the segment. If \iter is
 * `nullptr`, `false` is returned. The following example illustrates how to use
 * this:
 *
 * | const char *string = "foo/bar/baz"
 * | E_Str_Iter iter = e_cstr_split_init (string);
 * | while (e_cstr_split_char (&iter, '/')) {
 * |         printf ("Element: %.*s\n", (int) iter.len, iter.ptr);
 * | }
 */
bool e_cstr_split_char (E_Str_Iter *iter, char delim);

/**
 * Iterate on a string iterator \iter by splitting the string at \delim. See
 * `e_cstr_split_char` for details and an example. If \delim is `nullptr` or a
 * string of 0 length, this function essentially acts as an iterator over every
 * single character in the string.
 */
bool e_cstr_split_str (E_Str_Iter *iter, const char *delim);

/**
 * Iterate on a string iterator \iter by splitting the string at newline
 * characters. See `e_cstr_split_char` for details and an example.
 */
E_ATTR_REPRODUCIBLE
bool e_cstr_split_lines (E_Str_Iter *iter);

/**
 * Check if a nul-terminated string \s starts with the nul-terminated string
 * \expect. If \s is `nullptr`, `false` is returned. If \expect is `nullptr`,
 * `true` is returned.
 */
E_ATTR_REPRODUCIBLE
bool e_cstr_starts_with (const char *s, const char *expect);

/**
 * Check if a nul-terminated string \s ends with the nul-terminated string
 * \expect. If \s is `nullptr`, `false` is returned. If \expect is `nullptr`,
 * `true` is returned.
 */
E_ATTR_REPRODUCIBLE
bool e_cstr_ends_with (const char *s, const char *expect);

/**
 * Check if a nul-terminated string \s continues with the nul-terminated string
 * \expect at the position \pos. If \s is `nullptr` or \pos is outside of the
 * string, `false` is returned. If \expect is `nullptr`, `true` is returned.
 */
E_ATTR_REPRODUCIBLE
bool e_cstr_continues_with (const char *s, const char *expect, usize pos);

#endif /* E_CONFIG_MODULE_CSTR */
#endif /* _EMPOWER_CSTR_H_ */
