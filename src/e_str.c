#include "empower.h"

#if E_CONFIG_MODULE_STR

E_VEC_IMPLEMENT (char, __E_Vec_Char, __e_vec_char);

E_Str
e_str_init (void)
{
	E_Str str;

	str = __e_vec_char_init_with_cap (1);
	str.ptr[0] = 0;

	return str;
}

E_Str
e_str_init_with_cap (usize cap)
{
	E_Str str;

	str = __e_vec_char_init_with_cap (cap > 0 ? cap : 1);
	str.ptr[0] = 0;

	return str;
}

void
e_str_deinit (E_Str *str)
{
	__e_vec_char_deinit (str);
}

void
e_str_grow (E_Str *str, usize cap)
{
	__e_vec_char_grow (str, cap);
}

E_Str
e_str_clone (E_Str *str)
{
	return __e_vec_char_clone (str);
}

void
e_str_append_char (E_Str *str, char c)
{
	e_str_append_slice (str, &c, 1);
}

usize
e_str_append_cstr (E_Str *str, const char *s)
{
	usize len;

	len = strlen (s);
	e_str_append_slice (str, s, len);

	return len;
}

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

E_ATTR_FORMAT (printf, 3, 4)
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

E_ATTR_FORMAT (printf, 2, 3)
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

void
e_str_insert_char (E_Str *str, usize idx, char c)
{
	e_str_insert_slice (str, idx, &c, 1);
}

usize
e_str_insert_cstr (E_Str *str, usize idx, const char *s)
{
	usize len;

	len = strlen (s);
	e_str_insert_slice (str, idx, s, len);

	return len;
}

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
