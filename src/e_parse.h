#ifndef _EMPOWER_PARSE_H_
#define _EMPOWER_PARSE_H_

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

#include <stdbool.h>
#include <stdint.h>

/* public interface ***********************************************************/

/**
 * A pointer to a function that evaulates whether the ASCII character \c matches
 * a certain predicate. It returns 0 if \c is accepted, and non-zero if it is
 * rejected. The functions from `<ctype.h>` have this type.
 */
typedef int (* E_Parse_Char_Predicate) (int c);

const char *e_parse_char (const char **s, char c);
const char *e_parse_char_not (const char **s, char c);
const char *e_parse_char_pat (const char **s, const char *accept);
const char *e_parse_char_not_pat (const char **s, const char *reject);
const char *e_parse_char_func (const char **s, E_Parse_Char_Predicate func);
const char *e_parse_char_not_func (const char **s, E_Parse_Char_Predicate func);
const char *e_parse_str_match (const char **s, const char *match);
const char *e_parse_consume_while_char (const char **s, char c);
const char *e_parse_consume_while_pat (const char **s, const char *accept);
const char *e_parse_consume_while_func (const char **s, E_Parse_Char_Predicate func);
const char *e_parse_consume_until_char (const char **s, char c);
const char *e_parse_consume_until_pat (const char **s, const char *accept);
const char *e_parse_consume_until_func (const char **s, E_Parse_Char_Predicate func);
const char *e_parse_consume_line (const char **s);
const char *e_parse_consume_whitespace (const char **s);
const char *e_parse_consume_whitespace_until_newline (const char **s);
const char *e_parse_u8 (const char **s, uint8_t *out, int base);
const char *e_parse_u16 (const char **s, uint16_t *out, int base);
const char *e_parse_u32 (const char **s, uint32_t *out, int base);
const char *e_parse_u64 (const char **s, uint64_t *out, int base);
const char *e_parse_i8 (const char **s, int8_t *out, int base);
const char *e_parse_i16 (const char **s, int16_t *out, int base);
const char *e_parse_i32 (const char **s, int32_t *out, int base);
const char *e_parse_i64 (const char **s, int64_t *out, int base);
const char *e_parse_f32 (const char **s, float *out);
const char *e_parse_f64 (const char **s, double *out);
const char *e_parse_f128 (const char **s, long double *out);
const char *e_parse_bool (const char **s, bool *out);

/* implementation *************************************************************/

#ifdef E_PARSE_IMPL

#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const char *e_parse_priv_advance (const char **s, size_t n);

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched,
 * a pointer to the character (i.e. \s) is returned and \s is advanced. If \c is
 * not matched or \s is `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char (const char **s, char c)
{
	if (!s || !*s || **s == 0) return NULL;
	if (**s == c) return e_parse_priv_advance (s, 1);
	return NULL;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched, a pointer to the character (i.e. \s) is returned and \s is advanced.
 * If \c is matched or \s is `nullptr`, `nullptr` is returned and \s is not
 * advanced.
 */
const char *
e_parse_char_not (const char **s, char c)
{
	if (!s || !*s || **s == 0) return NULL;
	if (**s != c) return e_parse_priv_advance (s, 1);
	return NULL;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched
 * by (i.e. contained in) the pattern \accept, a pointer to the character (i.e.
 * \s) is returned and \s is advanced. If \c is not matched or either \s or
 * \accept are `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char_pat (const char **s, const char *accept)
{
	if (!s || !*s || **s == 0 || !accept) return NULL;
	if (strchr (accept, **s) != NULL) return e_parse_priv_advance (s, 1);
	return NULL;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched by (i.e. contained in) the pattern \reject or \reject is `nullptr`, a
 * pointer to the character (i.e. \s) is returned and \s is advanced. If \c is
 * matched or \s is `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char_not_pat (const char **s, const char *reject)
{
	if (!s || !*s || **s == 0) return NULL;
	if (!reject) return *s;
	if (strchr (reject, **s) == NULL) return e_parse_priv_advance (s, 1);
	return NULL;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is matched
 * by the E_Parse_Char_Predicate \func, a pointer to the character (i.e. \s) is
 * returned and \s is advanced. If \c is not matched or either \s or \func are
 * `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char_func (const char **s, E_Parse_Char_Predicate func)
{
	if (!s || !*s || **s == 0 || !func) return NULL;
	if (func (**s)) return e_parse_priv_advance (s, 1);
	return NULL;
}

/**
 * Parse a single character \c in a nul-terminated string \s. If \c is not
 * matched by the E_Parse_Char_Predicate \func, a pointer to the character (i.e.
 * \s) is returned and \s is advanced. If \c is matched or either \s or \func
 * are `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_char_not_func (const char **s, E_Parse_Char_Predicate func)
{
	if (!s || !*s || **s == 0 || !func) return NULL;
	if (!func (**s)) return e_parse_priv_advance (s, 1);
	return NULL;
}

/**
 * Parse a nul-terminated string \match in a nul-terminated string \s. If \match
 * the found at the start of \s, \s is advanced to after the match and a pointer
 * to the match (i.e. \s) is returned. If the string is not matched or either \s
 * or \match are `nullptr`, `nullptr` is returned and \s is not advanced.
 */
const char *
e_parse_str_match (const char **s, const char *match)
{
	size_t len;
	if (!s || !*s || **s == 0 || !match) return NULL;
	len = strlen (match);
	if (strncmp (*s, match, len) == 0) return e_parse_priv_advance (s, len);
	return NULL;
}

/**
 * Consume a nul-terminated string \s while a character \c is matched and
 * advance \s to the first character which is not matched. Returns \s.
 */
const char *
e_parse_consume_while_char (const char **s, char c)
{
	const char *r;
	if (!s || !*s) return NULL;
	r = *s;
	while (**s && **s == c) *s += 1;
	return r;
}

/**
 * Consume a nul-terminated string \s while a character is matched by (i.e.
 * contained in) the pattern \accept and advance \s to the first character which
 * is not matched. Returns \s.
 */
const char *
e_parse_consume_while_pat (const char **s, const char *accept)
{
	const char *r;
	if (!s || !*s) return NULL;
	if (!accept) return *s;
	r = *s;
	while (**s && strchr (accept, **s) != NULL) *s += 1;
	return r;
}

/**
 * Consume a nul-terminated string \s while a character is matched by the
 * E_Parse_Char_Predicate \func and advance \s to the first character which is
 * not matched. Returns \s.
 */
const char *
e_parse_consume_while_func (const char **s, E_Parse_Char_Predicate func)
{
	const char *r;
	if (!s || !*s) return NULL;
	if (!func) return *s;
	r = *s;
	while (**s && func (**s)) *s += 1;
	return r;
}

/**
 * Consume a nul-terminated string \s until a character \c is matched and
 * advance \s to the first character which is matched. Returns \s.
 */
const char *
e_parse_consume_until_char (const char **s, char c)
{
	const char *r;
	if (!s || !*s) return NULL;
	r = *s;
	while (**s && **s != c) *s += 1;
	return r;
}

/**
 * Consume a nul-terminated string \s until a character is matched by (i.e.
 * contained in) the pattern \accept and advance \s to the first character which
 * is matched. Returns \s.
 */
const char *
e_parse_consume_until_pat (const char **s, const char *accept)
{
	const char *r;
	if (!s || !*s) return NULL;
	if (!accept) return *s;
	r = *s;
	while (**s && strchr (accept, **s) == NULL) *s += 1;
	return r;
}

/**
 * Consume a nul-terminated string \s until a character is matched by the
 * E_Parse_Char_Predicate \func and advance \s to the first character which is
 * matched. Returns \s.
 */
const char *
e_parse_consume_until_func (const char **s, E_Parse_Char_Predicate func)
{
	const char *r;
	if (!s || !*s) return NULL;
	if (!func) return *s;
	r = *s;
	while (**s && !func (**s)) *s += 1;
	return r;
}

/**
 * Consume a nul-terminated string \s until the end of the line and advance \s
 * to the newline character or end of string. Returns \s. Note that the newline
 * character is not consumed and needs to be checked.
 */
const char *
e_parse_consume_line (const char **s)
{
	return e_parse_consume_until_char (s, '\n');
}

/**
 * Consume whitespace in a nul-terminated string \s and advance \s to the first
 * non-whitespace character. Returns \s.
 */
const char *
e_parse_consume_whitespace (const char **s)
{
	return e_parse_consume_while_func (s, isspace);
}


/**
 * Consume whitespace in a nul-terminated string \s and advance \s to the first
 * non-whitespace or newline character. Returns \s. Note that the newline
 * character is not consumed and needs to be checked:
 *
 *  | e_parse_consume_whitespace_until_newline (&s);
 *  | if (!e_parse_char (&s, '\n')) return FAIL ("unexpected trailing chars");
 */
const char *
e_parse_consume_whitespace_until_newline (const char **s)
{
	return e_parse_consume_while_pat (s, " \f\t\v\r");
}

/**
 * Parse an unsigned 8-bit integer from a nul-terminated string \s. The base of
 * the integer given in \base accepts the values 0 (auto-determine base based on
 * prefix of integer) or 2..36, just like the `strtol` family of functions.
 * Whitespace (as per `isspace`) at the beginning of the string is ignored. The
 * string will be "consumed" until the next non-digit character. If a valid
 * integer is found and it fits within the bounds of the desired integer type,
 * \s is advanced, the parsed integer is stored in \out and \s is returned. If
 * no integer is found or the integer is out of range, `nullptr` will be
 * returned, \s is not advanced, and \out will not be set to a value. If the
 * value is out of range, `errno` will be set to `ERANGE`.
 */
const char *
e_parse_u8 (const char **s, uint8_t *out, int base)
{
	const char *orig_s;
	char *num_end;
	unsigned long ret;

	if (!s || !*s || **s == 0) return NULL;
	errno = 0;
	ret = strtoul (*s, &num_end, base);
	if (errno != 0) return NULL; /* value out of range for strtoul */
	if (*s == num_end) return NULL; /* no valid number found */
	if (ret > UINT8_MAX) return errno = ERANGE, NULL;
	if (out) *out = (uint8_t) ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse an unsigned 16-bit integer from a nul-terminated string \s. See
 * `e_parse_u8` for details.
 */
const char *
e_parse_u16 (const char **s, uint16_t *out, int base)
{
	const char *orig_s;
	char *num_end;
	unsigned long ret;

	if (!s || !*s || **s == 0) return NULL;
	errno = 0;
	ret = strtoul (*s, &num_end, base);
	if (errno != 0) return NULL; /* value out of range for strtoul */
	if (*s == num_end) return NULL; /* no valid number found */
	if (ret > UINT16_MAX) return errno = ERANGE, NULL;
	if (out) *out = (uint16_t) ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse an unsigned 32-bit integer from a nul-terminated string \s. See
 * `e_parse_u8` for details.
 */
const char *
e_parse_u32 (const char **s, uint32_t *out, int base)
{
	const char *orig_s;
	char *num_end;
	unsigned long ret;

	if (!s || !*s || **s == 0) return NULL;
	errno = 0;
	ret = strtoul (*s, &num_end, base);
	if (errno != 0) return NULL; /* value out of range for strtoul */
	if (*s == num_end) return NULL; /* no valid number found */
	if (ret > UINT32_MAX) return errno = ERANGE, NULL;
	if (out) *out = (uint32_t) ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse an unsigned 64-bit integer from a nul-terminated string \s. See
 * `e_parse_u8` for details.
 */
const char *
e_parse_u64 (const char **s, uint64_t *out, int base)
{
	const char *orig_s;
	char *num_end;
	unsigned long long ret;

	if (!s || !*s || **s == 0) return NULL;
	errno = 0;
	ret = strtoull (*s, &num_end, base);
	if (errno != 0) return NULL; /* value out of range for strtoul */
	if (*s == num_end) return NULL; /* no valid number found */
	if (ret > UINT64_MAX) return errno = ERANGE, NULL;
	if (out) *out = (uint64_t) ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse a signed 8-bit integer from a nul-terminated string \s. See
 * `e_parse_u8` for details.
 */
const char *
e_parse_i8 (const char **s, int8_t *out, int base)
{
	const char *orig_s;
	char *num_end;
	signed long ret;

	if (!s || !*s || **s == 0) return NULL;
	errno = 0;
	ret = strtol (*s, &num_end, base);
	if (errno != 0) return NULL; /* value out of range for strtoul */
	if (*s == num_end) return NULL; /* no valid number found */
	if (ret < INT8_MIN || ret > INT8_MAX) return errno = ERANGE, NULL;
	if (out) *out = (int8_t) ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse a signed 16-bit integer from a nul-terminated string \s. See
 * `e_parse_u8` for details.
 */
const char *
e_parse_i16 (const char **s, int16_t *out, int base)
{
	const char *orig_s;
	char *num_end;
	signed long ret;

	if (!s || !*s || **s == 0) return NULL;
	errno = 0;
	ret = strtol (*s, &num_end, base);
	if (errno != 0) return NULL; /* value out of range for strtoul */
	if (*s == num_end) return NULL; /* no valid number found */
	if (ret < INT16_MIN || ret > INT16_MAX) return errno = ERANGE, NULL;
	if (out) *out = (int16_t) ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse a signed 32-bit integer from a nul-terminated string \s. See
 * `e_parse_u8` for details.
 */
const char *
e_parse_i32 (const char **s, int32_t *out, int base)
{
	const char *orig_s;
	char *num_end;
	signed long ret;

	if (!s || !*s || **s == 0) return NULL;
	errno = 0;
	ret = strtol (*s, &num_end, base);
	if (errno != 0) return NULL; /* value out of range for strtoul */
	if (*s == num_end) return NULL; /* no valid number found */
	if (ret < INT32_MIN || ret > INT32_MAX) return errno = ERANGE, NULL;
	if (out) *out = (int32_t) ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse a signed 64-bit integer from a nul-terminated string \s. See
 * `e_parse_u8` for details.
 */
const char *
e_parse_i64 (const char **s, int64_t *out, int base)
{
	const char *orig_s;
	char *num_end;
	signed long long ret;

	if (!s || !*s || **s == 0) return NULL;
	errno = 0;
	ret = strtoll (*s, &num_end, base);
	if (errno != 0) return NULL; /* value out of range for strtoul */
	if (*s == num_end) return NULL; /* no valid number found */
	if (ret < INT64_MIN || ret > INT64_MAX) return errno = ERANGE, NULL;
	if (out) *out = (int64_t) ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse a single-precision floating point number from a nul-terminated string
 * \s. This function works analogously to `e_parse_u8`, but for `float`s. The
 * parsing of the float literal works like `strtof`.
 */
const char *
e_parse_f32 (const char **s, float *out)
{
	const char *orig_s;
	char *num_end;
	float ret;

	if (!s || !*s || **s == 0) return NULL;
	ret = strtof (*s, &num_end);
	if (*s == num_end) return NULL; /* no valid number found */
	if (out) *out = ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse a double-precision floating point number from a nul-terminated string
 * \s. See `e_parse_f32` for details.
 */
const char *
e_parse_f64 (const char **s, double *out)
{
	const char *orig_s;
	char *num_end;
	double ret;

	if (!s || !*s || **s == 0) return NULL;
	ret = strtod (*s, &num_end);
	if (*s == num_end) return NULL; /* no valid number found */
	if (out) *out = ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

/*
 * Parse a long double-precision floating point number from a nul-terminated
 * string \s. See `e_parse_f32` for details.
 */
const char *
e_parse_f128 (const char **s, long double *out)
{
	const char *orig_s;
	char *num_end;
	long double ret;

	if (!s || !*s || **s == 0) return NULL;
	ret = strtold (*s, &num_end);
	if (*s == num_end) return NULL; /* no valid number found */
	if (out) *out = ret;
	orig_s = *s;
	*s = num_end;
	return orig_s;
}

// Parse a boolean value ("true" or "false") within the nul-terminated string
// \s. If a boolean value is found, \s will be returned, \s will be advanced to
// the character after the match and \out will be set to the value. If no
// boolean value is found, `nullptr` will be returned, \s will not be advanced
// and \out will not be filled.
const char *
e_parse_bool (const char **s, bool *out)
{
	if (!s || !*s || **s == 0) return NULL;

	if (strncmp (*s, "true", 4) == 0) {
		if (out) *out = true;
		return e_parse_priv_advance (s, 4);
	}

	if (strncmp (*s, "false", 5) == 0) {
		if (out) *out = false;
		return e_parse_priv_advance (s, 5);
	}

	return NULL;
}

static const char *
e_parse_priv_advance (const char **s, size_t n)
{
	const char *r;
	r = *s;
	*s += n;
	return r;
}

#endif /* E_PARSE_IMPL */

#endif /* _EMPOWER_PARSE_H_ */
