#include "empower.h"

#if E_CONFIG_MODULE_STR

E_VEC_IMPLEMENT (char, __e_vec_char);

e_str_t
e_str_init (void)
{
	return __e_vec_char_init ();
}

e_str_t
e_str_init_with_cap (usize cap)
{
	return __e_vec_char_init_with_cap (cap);
}

void
e_str_deinit (e_str_t *str)
{
	__e_vec_char_deinit (str);
}

void
e_str_grow (e_str_t *str, usize cap)
{
	__e_vec_char_grow (str, cap);
}

E_ATTR_ACCESS_READ_WRITE (1)
void
e_str_append_char (e_str_t *str, char c)
{
	if (!str) return;

	if (str->len + 1 > str->cap) {
		e_str_grow (str, str->len + 1);
	}

	str->ptr[str->len] = c;
	str->ptr[str->len + 1] = 0;
	str->len += 1;
}

E_ATTR_ACCESS_READ_WRITE (1)
E_ATTR_ACCESS_READ_ONLY (2)
E_ATTR_NUL_STRING_ARG (2)
usize
e_str_append_cstr (e_str_t *str, const char *s)
{
	usize len;

	len = strlen (s);
	e_str_append_slice (str, s, len);

	return len;
}

E_ATTR_ACCESS_READ_WRITE (1)
E_ATTR_ACCESS_READ_ONLY (2)
void
e_str_append_slice (e_str_t *str, const char *s, usize len)
{
	if (!str) return;

	if (str->len + len + 1 > str->cap) {
		e_str_grow (str, str->len + len + 1);
	}

	strncpy (&str->ptr[str->len], s, len);
	str->ptr[str->len + len] = 0;
	str->len += len;
}

E_ATTR_ACCESS_READ_WRITE (1)
E_ATTR_ACCESS_READ_ONLY (3)
E_ATTR_FORMAT (printf, 3, 4)
usize
e_str_append_fmt (e_str_t *str, usize max_fmt_len, const char *fmt, ...)
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

E_ATTR_ACCESS_READ_WRITE (1)
E_ATTR_ACCESS_READ_ONLY (2)
E_ATTR_FORMAT (printf, 2, 3)
usize
e_str_append_fmt_all (e_str_t *str, const char *fmt, ...)
{
	int written;
	va_list ap;

	if (!str) return 0;

#if E_STDC_VERSION >= E_STDC_VERSION_C99
	va_start (ap, fmt);
	written = vsnprintf (nullptr, 0, fmt, ap);
	va_end (ap);
#else /* E_STDC_VERSION >= E_STDC_VERSION_C99 */
	written = 1024; /* TODO: properly implement vsnprintf in C89 */
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

	if (str->len + (usize) written + 1 > str->cap) {
		e_str_grow (str, str->len + (usize) written + 1);
	}

	va_start (ap, fmt);
	written = vsnprintf (&str->ptr[str->len], (usize) written + 1, fmt, ap);
	va_end (ap);

	str->len += (usize) written;

	return (usize) written;
}

#endif /* E_CONFIG_MODULE_STR */
