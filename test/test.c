#define E_TEST_MAIN
#define E_TEST_TYPE_MACROS
#include "empower.h"
#include <ctype.h>

static void test_cstr (void);

int
main (int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	test_cstr ();

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
