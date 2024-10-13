#include "e_str.h"

#if E_CONFIG_MODULE_STR

#include <stdarg.h>

E_VEC_IMPLEMENT (char, __E_Vec_Char, __e_vec_char);

/**
 * Initialise a dynamically sized string with capacity 0. This function
 * allocates enough memory to store a string with a nul terminating byte. When
 * the allocation fails, it will terminate the programme with an error message.
 * The user must free the allocated memory with `e_str_free`.
 */
E_Str
e_str_init (void)
{
	E_Str str;

	str = __e_vec_char_init_with_cap (1);
	str.ptr[0] = 0;

	return str;
}

/**
 * Initialise a dynamically sized string with a given capacity. This function
 * allocates enough memory to store \cap characters (including the terminating
 * nul character). When the allocation fails, it will terminate the programme
 * with an error message. The user must free the allocated memory with
 * `e_str_free`.
 */
E_Str
e_str_init_with_cap (usize cap)
{
	E_Str str;

	str = __e_vec_char_init_with_cap (cap > 0 ? cap : 1);
	str.ptr[0] = 0;

	return str;
}

/**
 * Deinitialise a dynamically sized string and free the memory occupied by it.
 */
void
e_str_deinit (E_Str *str)
{
	__e_vec_char_deinit (str);
}

/**
 * Grow the memory of a dynamically sized string \str to at least \cap items
 * (including the terminating nul character). If \cap is lower than the current
 * capacity of the string, no action will be performed. Internally, it rounds up
 * \cap to the nearest power of two and reallocates the memory to the new
 * capacity.
 */
void
e_str_grow (E_Str *str, usize cap)
{
	__e_vec_char_grow (str, cap);
}

/**
 * Clone a string. This function allocates memory which must be freed by the
 * user using \e_str_free. If \str is `nullptr`, an empty string is returned.
 */
E_Str
e_str_clone (E_Str *str)
{
	return __e_vec_char_clone (str);
}

/**
 * Append a single character \c to the string \str and reallocate if necessary.
 */
void
e_str_append_char (E_Str *str, char c)
{
	e_str_append_slice (str, &c, 1);
}

/**
 * Append a nul-terminated string \s to the string \str and reallocate if
 * necessary. Returns `strlen (s)`.
 */
usize
e_str_append_cstr (E_Str *str, const char *s)
{
	usize len;

	len = strlen (s);
	e_str_append_slice (str, s, len);

	return len;
}

/**
 * Append a string \s of length \len to the string \str and reallocate if
 * necessary.
 */
void
e_str_append_slice (E_Str *str, const char *s, usize len)
{
	if (!str) return;

	if (str->len + len + 1 > str->cap) {
		e_str_grow (str, str->len + len + 1);
	}

	strncpy (&str->ptr[str->len], s, len);
	str->ptr[str->len + len] = 0;
	str->len += len;
}

/**
 * Append a formatted string to the string \str and reallocate if necessary.
 * This function works using `vsnprintf`: \fmt serves as the formatting
 * specifier, and only up to \max_fmt_len bytes are actually written to the
 * string. Returns the number of bytes that would have been written if the
 * entire formatted string could have been appended. If the entire string was
 * appended, this is equal to the number of bytes actually written.
 */
C_ATTR_FORMAT (printf, 3, 4)
usize
e_str_append_fmt (E_Str *str, usize max_fmt_len, const char *fmt, ...)
{
	int written;
	va_list ap;

	if (!str) return 0;

	if (str->len + max_fmt_len + 1 > str->cap) {
		e_str_grow (str, str->len + max_fmt_len + 1);
	}

	va_start (ap, fmt);
	written = vsnprintf (&str->ptr[str->len], max_fmt_len + 1, fmt, ap);
	va_end (ap);

	str->len += (usize) written < max_fmt_len
		? (usize) written
		: max_fmt_len;

	return (usize) written;
}


/**
 * Append a formatted string to the string \str. The required space is
 * determined automatically and the string is resized accordingly. tReturns he
 * number of bytes that were written to the string.
 */
C_ATTR_FORMAT (printf, 2, 3)
usize
e_str_append_fmt_all (E_Str *str, const char *fmt, ...)
{
	int written;
	va_list ap;

	if (!str) return 0;

	va_start (ap, fmt);
	written = vsnprintf (nullptr, 0, fmt, ap);
	va_end (ap);

	if (str->len + (usize) written + 1 > str->cap) {
		e_str_grow (str, str->len + (usize) written + 1);
	}

	va_start (ap, fmt);
	written = vsnprintf (&str->ptr[str->len], (usize) written + 1, fmt, ap);
	va_end (ap);

	str->len += (usize) written;

	return (usize) written;
}

/**
 * Insert a single character \c at position \idx into the string \str. If \idx
 * is out of bounds, the character is appended instead. When inserting into the
 * middle of the string, the rest of the string is pushed back. The memory is
 * reallocated if necessary.
 */
void
e_str_insert_char (E_Str *str, usize idx, char c)
{
	e_str_insert_slice (str, idx, &c, 1);
}

/**
 * Insert a nul-terminated string \s at position \idx into the string \str. If
 * \idx is out of bounds, the character is appended instead. When inserting into
 * the middle of the string, the rest of the string is pushed back. The memory
 * is reallocated if necessary. Retruns the number of characters added to \str,
 * i.e. the length of \s.
 */
usize
e_str_insert_cstr (E_Str *str, usize idx, const char *s)
{
	usize len;

	len = strlen (s);
	e_str_insert_slice (str, idx, s, len);

	return len;
}

/**
 * Insert a string \s of length \len at position \idx into the string \str. If
 * \idx is out of bounds, the character is appended instead. When inserting into
 * the middle of the string, the rest of the string is pushed back. The memory
 * is reallocated if necessary.
 */
void
e_str_insert_slice (E_Str *str, usize idx, const char *s, usize len)
{
	if (!str || !s || len == 0) return;
	if (idx >= str->len) {
		e_str_append_slice (str, s, len);
		return;
	}

	if (str->len + len + 1 > str->cap) {
		e_str_grow (str, str->len + len + 1);
	}

	memmove (&str->ptr[idx + len], &str->ptr[idx], str->len - idx + 1);
	strncpy (&str->ptr[idx], s, len);
	str->len += len;
}

/**
 * Remove \count characters from the string \str, starting at \idx. Returns the
 * number of characters removed. When removing from the middle of the stirng,
 * the memory inside the string is shifted accordingly.
 */
usize
e_str_remove (E_Str *str, usize idx, usize count)
{
	if (!str || count == 0 || idx >= str->len) return 0;
	if (idx + count == str->len) {
		str->len -= count;
		str->ptr[str->len] = 0;
		return count;
	}
	count = idx + count > str->len ? str->len - idx : count;
	memmove (&str->ptr[idx], &str->ptr[idx + count],
	         str->len - idx - count + 1);
	str->len -= count;
	str->ptr[str->len] = 0;
	return count;
}

#endif /* E_CONFIG_MODULE_STR */
