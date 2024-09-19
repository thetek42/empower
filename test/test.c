#define E_CONFIG_TEST_TYPE_MACROS_OVERRIDE
#include "empower.h"
#include <ctype.h>

static void test_cstr (void);
static void test_str (void);

E_TEST_MAIN ();

int
main (int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	test_cstr ();
	test_str ();

	e_test_finish ();
}

static void
test_cstr (void)
{
	char s1[] = "Hello, World!";
	char s2[] = " \n\t \f \v    \r ";
	char s3[] = "1337";
	char s4[] = "Dosenöffner";

	e_test_assert_eq ("e_cstr_count_char", usize, e_cstr_count_char (s1, 'l'), 3);
	e_test_assert_eq ("e_cstr_count_char nul", usize, e_cstr_count_char (s1, '\0'), 0);
	e_test_assert_eq ("e_cstr_count_func", usize, e_cstr_count_func (s1, islower), 8);
	e_test_assert ("e_cstr_is_blank false", bool, !e_cstr_is_blank (s1));
	e_test_assert ("e_cstr_is_blank true", bool, e_cstr_is_blank (s2));
	e_test_assert ("e_cstr_is_ascii false", bool, !e_cstr_is_ascii (s4));
	e_test_assert ("e_cstr_is_ascii true", bool, e_cstr_is_ascii (s1));
	e_test_assert ("e_cstr_matches_predicate false", bool, !e_cstr_matches_predicate (s3, islower));
	e_test_assert ("e_cstr_matches_predicate true", bool, e_cstr_matches_predicate (s3, isdigit));
	e_test_assert_str_eq ("e_cstr_to_ascii_lower", e_cstr_to_ascii_lower (s1), "hello, world!");
	e_test_assert_str_eq ("e_cstr_to_ascii_upper", e_cstr_to_ascii_upper (s1), "HELLO, WORLD!");
}

static void
test_str (void)
{
	e_str_t str;
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

	len_ret = e_str_append_fmt (&str, 32, " dec:%d,hex:%x", 42, 42);
	e_test_assert_str_eq ("e_str_append_fmt", str.ptr, "Hello, World! 123 dec:42,hex:2a");
	e_test_assert_eq ("e_str_append_fmt len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a"));
	e_test_assert_eq ("e_str_append_fmt len_ret", usize, len_ret, strlen (" dec:42,hex:2a"));

#if E_STDC_VERSION >= E_STDC_VERSION_C99
	len_ret = e_str_append_fmt (&str, 4, " %d", 12345);
	e_test_assert_str_eq ("e_str_append_fmt capped", str.ptr, "Hello, World! 123 dec:42,hex:2a 123");
	e_test_assert_eq ("e_str_append_fmt capped len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a 123"));
	e_test_assert_eq ("e_str_append_fmt capped len_ret", usize, len_ret, 6);

	len_ret = e_str_append_fmt_all (&str, " %d", 12345);
	e_test_assert_str_eq ("e_str_append_fmt_all", str.ptr, "Hello, World! 123 dec:42,hex:2a 123 12345");
	e_test_assert_eq ("e_str_append_fmt_all len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a 123 12345"));
	e_test_assert_eq ("e_str_append_fmt_all len_ret", usize, len_ret, 6);
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

	e_str_deinit (&str);
}
