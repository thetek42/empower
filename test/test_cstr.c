#define E_CSTR_IMPL
#include <e_cstr.h>

#include <e_test.h>
#include <e_types.h>

void
test_cstr (void)
{
	char s1[] = "Hello, World!";
	char s2[] = " \n\t \f \v    \r ";
	char s3[] = "1337";
	char s4[] = "Dosenöffner öffnen Dosen.";
	char s5[] = "  foo   ";
	char s6[] = "";
	char s7[] = "  foo   ";
	char s8[] = "fooXXbarXXXXbazXX";
	usize len;
	E_Str_Split split;

	e_test_assert_eq ("e_cstr_count_char", usize, e_cstr_count_char (s1, 'l'), 3);
	e_test_assert_eq ("e_cstr_count_char nul", usize, e_cstr_count_char (s1, '\0'), 0);
	e_test_assert_eq ("e_cstr_count_char_not", usize, e_cstr_count_char_not (s1, 'l'), strlen (s1) - 3);
	e_test_assert_eq ("e_cstr_count_char_pat", usize, e_cstr_count_char_pat (s1, "load"), 6);
	e_test_assert_eq ("e_cstr_count_char_not_pat", usize, e_cstr_count_char_not_pat (s1, "load"), strlen (s1) - 6);
	e_test_assert_eq ("e_cstr_count_char_func", usize, e_cstr_count_char_func (s1, islower), 8);
	e_test_assert_eq ("e_cstr_count_char_not_func", usize, e_cstr_count_char_not_func (s1, islower), strlen (s1) - 8);
	e_test_assert_eq ("e_cstr_count_str", usize, e_cstr_count_str (s8, "XX"), 4);
	e_test_assert_eq ("e_cstr_count_str too long", usize, e_cstr_count_str (s3, "123456"), 0);
	e_test_assert_eq ("e_cstr_count_str_overlap", usize, e_cstr_count_str_overlap (s8, "XX"), 5);
	e_test_assert_eq ("e_cstr_count_lines 1", usize, e_cstr_count_lines (s1), 1);
	e_test_assert_eq ("e_cstr_count_lines 2", usize, e_cstr_count_lines (s2), 2);
	e_test_assert_eq ("e_cstr_len", usize, e_cstr_len (s3), strlen (s3));
	e_test_assert_eq ("e_cstr_len nullptr", usize, e_cstr_len (nullptr), 0);
	e_test_assert ("e_cstr_is_blank false", !e_cstr_is_blank (s1));
	e_test_assert ("e_cstr_is_blank true", e_cstr_is_blank (s2));
	e_test_assert ("e_cstr_is_ascii false", !e_cstr_is_ascii (s4));
	e_test_assert ("e_cstr_is_ascii true", e_cstr_is_ascii (s1));
	e_test_assert_ptr_eq ("e_cstr_find_char existant", e_cstr_find_char (s1, 'o'), s1 + 4);
	e_test_assert_null ("e_cstr_find_char nonexistant", e_cstr_find_char (s1, 'u'));
	e_test_assert_ptr_eq ("e_cstr_find_char_not", e_cstr_find_char_not (s1, 'H'), s1 + 1);
	e_test_assert_ptr_eq ("e_cstr_find_char_pat existant", e_cstr_find_char_pat (s1, "abcd"), s1 + 11);
	e_test_assert_null ("e_cstr_find_char_pat nonexistant", e_cstr_find_char_pat (s1, "xyz"));
	e_test_assert_ptr_eq ("e_cstr_find_char_not_pat existant", e_cstr_find_char_not_pat (s1, "HWelo, "), s1 + 9);
	e_test_assert_null ("e_cstr_find_char_not_pat nonexistant", e_cstr_find_char_not_pat (s3, "1234567890"));
	e_test_assert_ptr_eq ("e_cstr_find_char_func existant", e_cstr_find_char_func (s5, isalpha), s5 + 2);
	e_test_assert_null ("e_cstr_find_char_func nonexistant", e_cstr_find_char_func (s5, isdigit));
	e_test_assert_ptr_eq ("e_cstr_find_char_not_func existant", e_cstr_find_char_not_func (s5, isspace), s5 + 2);
	e_test_assert_null ("e_cstr_find_char_not_func nonexistant", e_cstr_find_char_not_func (s3, isdigit));
	e_test_assert_ptr_eq ("e_cstr_find_str existant", e_cstr_find_str (s4, "en"), s4 + 3);
	e_test_assert_null ("e_cstr_find_str nonexistant", e_cstr_find_str (s5, "es"));
	e_test_assert_ptr_eq ("e_cstr_rfind_char existant", e_cstr_rfind_char (s1, 'o'), s1 + 8);
	e_test_assert_null ("e_cstr_rfind_char nonexistant", e_cstr_rfind_char (s1, 'u'));
	e_test_assert_ptr_eq ("e_cstr_rfind_char_not", e_cstr_rfind_char_not (s1, '!'), s1 + 11);
	e_test_assert_ptr_eq ("e_cstr_rfind_char_pat existant", e_cstr_rfind_char_pat (s1, "elo"), s1 + 10);
	e_test_assert_null ("e_cstr_rfind_char_pat nonexistant", e_cstr_rfind_char_pat (s1, "xyz"));
	e_test_assert_ptr_eq ("e_cstr_rfind_char_not_pat existant", e_cstr_rfind_char_not_pat (s1, "du!"), s1 + 10);
	e_test_assert_null ("e_cstr_rfind_char_not_pat nonexistant", e_cstr_rfind_char_not_pat (s3, "1234567890"));
	e_test_assert_ptr_eq ("e_cstr_rfind_char_func existant", e_cstr_rfind_char_func (s5, isalpha), s5 + 4);
	e_test_assert_null ("e_cstr_rfind_char_func nonexistant", e_cstr_rfind_char_func (s5, isdigit));
	e_test_assert_ptr_eq ("e_cstr_rfind_char_not_func existant", e_cstr_rfind_char_not_func (s5, isspace), s5 + 4);
	e_test_assert_null ("e_cstr_rfind_char_not_func nonexistant", e_cstr_rfind_char_not_func (s3, isdigit));
	e_test_assert_ptr_eq ("e_cstr_rfind_str existant", e_cstr_rfind_str (s4, "en"), s4 + 24);
	e_test_assert_null ("e_cstr_rfind_str nonexistant", e_cstr_rfind_str (s5, "es"));
	e_test_assert ("e_cstr_eq null null", e_cstr_eq (nullptr, nullptr));
	e_test_assert ("e_cstr_eq ptr null", !e_cstr_eq (s5, nullptr));
	e_test_assert ("e_cstr_eq ptr eq ptr", e_cstr_eq (s5, s5));
	e_test_assert ("e_cstr_eq eq", e_cstr_eq (s5, s7));
	e_test_assert ("e_cstr_eq not eq", !e_cstr_eq (s5, s4));
	e_test_assert ("e_cstr_matches_predicate false", !e_cstr_matches_predicate (s3, islower));
	e_test_assert ("e_cstr_matches_predicate true", e_cstr_matches_predicate (s3, isdigit));
	e_test_assert_ptr_eq ("e_cstr_trim_start", e_cstr_trim_start (s5), s5 + 2);
	e_test_assert_ptr_eq ("e_cstr_trim_start empty", e_cstr_trim_start (s6), s6);
	e_test_assert_eq ("e_cstr_trim_end", usize, e_cstr_trim_end (s5), strlen (s5) - 3);
	e_test_assert_eq ("e_cstr_trim_end empty", usize, e_cstr_trim_end (s6), 0);
	e_test_assert_ptr_eq ("e_cstr_trim", e_cstr_trim (s5, &len), s5 + 2);
	e_test_assert_eq ("e_cstr_trim len", usize, len, 3);
	e_test_assert_ptr_eq ("e_cstr_trim empty", e_cstr_trim (s6, &len), s6);
	e_test_assert_eq ("e_cstr_trim empty len", usize, len, 0);
	e_test_assert ("e_cstr_starts_with true", e_cstr_starts_with (s1, "Hel"));
	e_test_assert ("e_cstr_starts_with false", !e_cstr_starts_with (s1, "foo"));
	e_test_assert ("e_cstr_ends_with true", e_cstr_ends_with (s1, "World!"));
	e_test_assert ("e_cstr_ends_with false", !e_cstr_starts_with (s1, "foo"));
	e_test_assert ("e_cstr_continues_with true", e_cstr_continues_with (s1, "lo,", 3));
	e_test_assert ("e_cstr_continues_with false", !e_cstr_continues_with (s1, "foo", 3));
	e_test_assert_str_eq ("e_cstr_to_ascii_lower", e_cstr_to_ascii_lower (s1), "hello, world!");
	e_test_assert_str_eq ("e_cstr_to_ascii_upper", e_cstr_to_ascii_upper (s1), "HELLO, WORLD!");
	e_test_assert_eq ("e_cstr_distance", usize, e_cstr_distance ("kitten", "sitting"), 3);

	split = e_cstr_split_char ("/foo//bar/baz", '/');
	e_test_assert_eq ("e_cstr_split_char num_items", usize, split.num_items, 5);
	e_test_assert_str_eq ("e_cstr_split_char next 1", e_cstr_split_next (&split), "");
	e_test_assert_str_eq ("e_cstr_split_char next 2", e_cstr_split_next (&split), "foo");
	e_test_assert_str_eq ("e_cstr_split_char next 3", e_cstr_split_next (&split), "");
	e_test_assert_str_eq ("e_cstr_split_char next 4", e_cstr_split_next (&split), "bar");
	e_test_assert_str_eq ("e_cstr_split_char next 5", e_cstr_split_next (&split), "baz");
	e_test_assert_null ("e_cstr_split_char next 6", e_cstr_split_next (&split));
	e_cstr_split_deinit (&split);

	split = e_cstr_split_str ("XXfooXXXbarXXXXbaz", "XX");
	e_test_assert_eq ("e_cstr_split_str num_items", usize, split.num_items, 5);
	e_test_assert_str_eq ("e_cstr_split_str next 1", e_cstr_split_next (&split), "");
	e_test_assert_str_eq ("e_cstr_split_str next 2", e_cstr_split_next (&split), "foo");
	e_test_assert_str_eq ("e_cstr_split_str next 3", e_cstr_split_next (&split), "Xbar");
	e_test_assert_str_eq ("e_cstr_split_str next 4", e_cstr_split_next (&split), "");
	e_test_assert_str_eq ("e_cstr_split_str next 5", e_cstr_split_next (&split), "baz");
	e_test_assert_null ("e_cstr_split_str next 6", e_cstr_split_next (&split));
	e_cstr_split_deinit (&split);
}
