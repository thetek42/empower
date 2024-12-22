#define E_STR_IMPL
#include <e_str.h>

#include <e_test.h>
#include <e_types.h>

void
test_str (void)
{
	E_Str str;
	usize len_ret;

	str = e_str_init ();

	len_ret = e_str_append_cstr (&str, "Hello, World");
	e_test_assert_str_eq ("e_str_append_cstr", str.ptr, "Hello, World");
	e_test_assert_eq ("e_str_append_cstr len", usize, str.len, strlen ("Hello, World"));
	e_test_assert_eq ("e_str_append_cstr len_ret", usize, len_ret, strlen ("Hello, World"));

	e_str_append_char (&str, '!');
	e_test_assert_str_eq ("e_str_append_char", str.ptr, "Hello, World!");
	e_test_assert_eq ("e_str_append_char len", usize, str.len, strlen ("Hello, World!"));

	e_str_append_slice (&str, " 12345", 4);
	e_test_assert_str_eq ("e_str_append_slice", str.ptr, "Hello, World! 123");
	e_test_assert_eq ("e_str_append_char len", usize, str.len, strlen ("Hello, World! 123"));

	len_ret = e_str_append_fmt_n (&str, 32, " dec:%d,hex:%x", 42, 42);
	e_test_assert_str_eq ("e_str_append_fmt_n", str.ptr, "Hello, World! 123 dec:42,hex:2a");
	e_test_assert_eq ("e_str_append_fmt_n len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a"));
	e_test_assert_eq ("e_str_append_fmt_n len_ret", usize, len_ret, strlen (" dec:42,hex:2a"));

	len_ret = e_str_append_fmt_n (&str, 4, " %d", 12345);
	e_test_assert_str_eq ("e_str_append_fmt_n capped", str.ptr, "Hello, World! 123 dec:42,hex:2a 123");
	e_test_assert_eq ("e_str_append_fmt_n capped len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a 123"));
	e_test_assert_eq ("e_str_append_fmt_n capped len_ret", usize, len_ret, 6);

	len_ret = e_str_append_fmt (&str, " %d", 12345);
	e_test_assert_str_eq ("e_str_append_fmt", str.ptr, "Hello, World! 123 dec:42,hex:2a 123 12345");
	e_test_assert_eq ("e_str_append_fmt len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a 123 12345"));
	e_test_assert_eq ("e_str_append_fmt len_ret", usize, len_ret, 6);

	e_str_insert_char (&str, 3, 'X');
	e_test_assert_str_eq ("e_str_insert_char", str.ptr, "HelXlo, World! 123 dec:42,hex:2a 123 12345");
	e_test_assert_eq ("e_str_insert_char len", usize, str.len, strlen ("HelXlo, World! 123 dec:42,hex:2a 123 12345"));

	e_str_insert_cstr (&str, 4, "YZ");
	e_test_assert_str_eq ("e_str_insert_cstr", str.ptr, "HelXYZlo, World! 123 dec:42,hex:2a 123 12345");
	e_test_assert_eq ("e_str_insert_cstr len", usize, str.len, strlen ("HelXYZlo, World! 123 dec:42,hex:2a 123 12345"));

	e_str_insert_slice (&str, 6, "ABC", 3);
	e_test_assert_str_eq ("e_str_insert_slice", str.ptr, "HelXYZABClo, World! 123 dec:42,hex:2a 123 12345");
	e_test_assert_eq ("e_str_insert_slice len", usize, str.len, strlen ("HelXYZABClo, World! 123 dec:42,hex:2a 123 12345"));

	e_str_insert_slice (&str, 1000, "ABC", 3);
	e_test_assert_str_eq ("e_str_insert_slice end", str.ptr, "HelXYZABClo, World! 123 dec:42,hex:2a 123 12345ABC");
	e_test_assert_eq ("e_str_insert_slice end len", usize, str.len, strlen ("HelXYZABClo, World! 123 dec:42,hex:2a 123 12345ABC"));

	len_ret = e_str_remove (&str, 10, 10);
	e_test_assert_str_eq ("e_str_remove", str.ptr, "HelXYZABCl123 dec:42,hex:2a 123 12345ABC");
	e_test_assert_eq ("e_str_remove len", usize, str.len, strlen ("HelXYZABCl123 dec:42,hex:2a 123 12345ABC"));
	e_test_assert_eq ("e_str_remove len_ret", usize, len_ret, 10);

	len_ret = e_str_remove (&str, 35, 10);
	e_test_assert_str_eq ("e_str_remove end", str.ptr, "HelXYZABCl123 dec:42,hex:2a 123 123");
	e_test_assert_eq ("e_str_remove end len", usize, str.len, strlen ("HelXYZABCl123 dec:42,hex:2a 123 123"));
	e_test_assert_eq ("e_str_remove end len_ret", usize, len_ret, 5);

	e_test_assert ("e_str_remove_prefix true ret", e_str_remove_prefix (&str, "Hel"));
	e_test_assert_str_eq ("e_str_remove_prefix true ptr", str.ptr, "XYZABCl123 dec:42,hex:2a 123 123");
	e_test_assert_eq ("e_str_remove_prefix true len", usize, str.len, strlen ("XYZABCl123 dec:42,hex:2a 123 123"));
	e_test_assert ("e_str_remove_prefix false ret", !e_str_remove_prefix (&str, "Hel"));
	e_test_assert_str_eq ("e_str_remove_prefix false ptr", str.ptr, "XYZABCl123 dec:42,hex:2a 123 123");
	e_test_assert_eq ("e_str_remove_prefix false len", usize, str.len, strlen ("XYZABCl123 dec:42,hex:2a 123 123"));
	e_test_assert ("e_str_remove_suffix true ret", e_str_remove_suffix (&str, "123 123"));
	e_test_assert_str_eq ("e_str_remove_suffix true ptr", str.ptr, "XYZABCl123 dec:42,hex:2a ");
	e_test_assert_eq ("e_str_remove_suffix true len", usize, str.len, strlen ("XYZABCl123 dec:42,hex:2a "));
	e_test_assert ("e_str_remove_suffix false ret", !e_str_remove_suffix (&str, "123 123"));
	e_test_assert_str_eq ("e_str_remove_suffix false ptr", str.ptr, "XYZABCl123 dec:42,hex:2a ");
	e_test_assert_eq ("e_str_remove_suffix false len", usize, str.len, strlen ("XYZABCl123 dec:42,hex:2a "));

	len_ret = e_str_remove (&str, 0, str.len);
	e_test_assert_str_eq ("e_str_remove entire", str.ptr, "");
	e_test_assert_eq ("e_str_remove entire len", usize, str.len, 0);
	e_test_assert_eq ("e_str_remove entire len_ret", usize, len_ret, strlen ("XYZABCl123 dec:42,hex:2a "));

	e_str_deinit (&str);

	str = e_str_from_cstr ("   foo bar baz   ");
	e_str_trim_start (&str);
	e_test_assert_str_eq ("e_str_trim_start ptr", str.ptr, "foo bar baz   ");
	e_test_assert_eq ("e_str_trim_start len", usize, str.len, strlen ("foo bar baz   "));
	e_str_deinit (&str);

	str = e_str_from_cstr ("   foo bar baz   ");
	e_str_trim_end (&str);
	e_test_assert_str_eq ("e_str_trim_end ptr", str.ptr, "   foo bar baz");
	e_test_assert_eq ("e_str_trim_end len", usize, str.len, strlen ("   foo bar baz"));
	e_str_deinit (&str);

	str = e_str_from_cstr ("   foo bar baz   ");
	e_str_trim (&str);
	e_test_assert_str_eq ("e_str_trim ptr", str.ptr, "foo bar baz");
	e_test_assert_eq ("e_str_trim len", usize, str.len, strlen ("foo bar baz"));
	e_str_deinit (&str);
}
