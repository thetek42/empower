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
 * a certain predicate. The functions from `<ctype.h>` have this type.
 *
 * @param c: The character to check
 * @return 0 when \c is accepted, a different number otherwise.
 */
typedef int (* e_char_predicate_t) (int c);

/**
 * Count the number of occurances of a character \c in the string \s.
 *
 * @param s #cstr: The nul-terminated string to check
 * @param c: The character to search for
 * @return The number of occurances of \c in \s, or 0 if \s is `nullptr`.
 */
E_ATTR_PURE ()
usize e_cstr_count_char (const char *s, char c);

/**
 * Count the number of matches of a \e_char_predicate_t \func in the string \s.
 *
 * @param s #cstr: The nul-terminated string to check
 * @param c: The predicate for which characters to count
 * @return The number of matches of \func in \s, or 0 if either \s or \func is
 *         `nullptr`.
 */
usize e_cstr_count_func (const char *s, e_char_predicate_t func);

/**
 * Check if the string \s is ASCII.
 *
 * @param s #cstr: The nul-terminated string to check
 * @return `true` when \s is `nullptr` or only consists of ASCII characters
 */
E_ATTR_PURE ()
bool e_cstr_is_ascii (const char *s);

/**
 * Check if the string \s only consists of whitespace characters.
 *
 * @param s #cstr: The nul-terminated string to check
 * @return `true` when \s is `nullptr` or only consists of whitespace
 * @see `isspace`
 */
E_ATTR_PURE ()
bool e_cstr_is_blank (const char *s);

/**
 * Check if the string \s matches the \e_char_predicate_t \func.
 *
 * @param s #cstr: The nul-terminated string to check
 * @param func: The predicate for which characters are accepted
 * @return `true` when \s is `nullptr` or only consists of characters that were
 *         accepted by \func, `false` when \func is `nullptr` or did not match.
 */
bool e_cstr_matches_predicate (const char *s, e_char_predicate_t func);

/**
 * Convert uppercase ASCII characters the string \s to lowercase in-place. When
 * \s is a `nullptr`, no action is performed.
 *
 * @param s #cstr: The nul-terminated string to convert
 * @return \s
 */
char *e_cstr_to_ascii_lower (char *s);

/**
 * Convert lowercase ASCII characters the string \s to uppercase in-place. When
 * \s is a `nullptr`, no action is performed.
 *
 * @param s #cstr: The nul-terminated string to convert
 * @return \s
 */
char *e_cstr_to_ascii_upper (char *s);

/**
 * Convert uppercase ASCII characters the string \src to lowercase and store
 * them in \dest. The terminating nul byte in \dest is written. When \src or
 * \dest are `nullptr`s, no action is performed.
 *
 * @param src #cstr: The nul-terminated string source
 * @param dest: The destination for the converted string
 * @return \dest
 * @note \dest must have enough space to store all of \src, including the
 *       terminating nul byte.
 */
char *e_cstr_to_ascii_lower_buf (const char *restrict src, char *restrict dest);

/**
 * Convert uppercase ASCII characters the string \src to lowercase and store
 * them in \dest. The terminating nul byte in \dest is written. When \src or
 * \dest are `nullptr`s, no action is performed.
 *
 * @param src #cstr: The nul-terminated string source
 * @param dest: The destination for the converted string
 * @return \dest
 * @note \dest must have enough space to store all of \src, including the
 *       terminating nul byte.
 */
char *e_cstr_to_ascii_upper_buf (const char *restrict src, char *restrict dest);

/**
 * Check if the character \c is ASCII.
 *
 * @param c: The character to check
 * @return 0 if \c is ASCII, a different number otherwise.
 * @see `isascii` from `<ctype.h>` on POSIX systems
 */
E_ATTR_CONST ()
int e_char_is_ascii (int c);

#endif /* E_CONFIG_MODULE_CSTR */
#endif /* _EMPOWER_CSTR_H_ */
