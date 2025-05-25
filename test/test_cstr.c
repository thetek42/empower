#define E_CSTR_IMPL
#include <e_cstr.h>

#include <e_test.h>
#include <e_types.h>

static void test_cstr_count (void);
static void test_cstr_len (void);
static void test_cstr_eq (void);
static void test_cstr_matches (void);
static void test_cstr_find (void);
static void test_cstr_trim (void);
static void test_cstr_upper_lower (void);
static void test_cstr_distance (void);
static void test_cstr_split (void);

void
test_cstr (void)
{
	test_cstr_count ();
	test_cstr_len ();
	test_cstr_eq ();
	test_cstr_matches ();
	test_cstr_find ();
	test_cstr_trim ();
	test_cstr_upper_lower ();
	test_cstr_distance ();
	test_cstr_split ();
}

static void
test_cstr_count (void)
{
	e_test_assert_eq ("e_cstr_count_char", usize, e_cstr_count_char ("Hello, World!", 'l'), 3);
	e_test_assert_eq ("e_cstr_count_char none", usize, e_cstr_count_char ("Hello, World!", 'x'), 0);
	e_test_assert_eq ("e_cstr_count_char nul", usize, e_cstr_count_char ("Hello, World!", '\0'), 0);
	e_test_assert_eq ("e_cstr_count_char empty", usize, e_cstr_count_char ("", 'x'), 0);
	e_test_assert_eq ("e_cstr_count_char empty nul", usize, e_cstr_count_char ("", '\0'), 0);
	e_test_assert_eq ("e_cstr_count_char all", usize, e_cstr_count_char ("xxx", 'x'), 3);

	e_test_assert_eq ("e_cstr_count_char_not", usize, e_cstr_count_char_not ("Hello, World!", 'l'), 10);
	e_test_assert_eq ("e_cstr_count_char_not none", usize, e_cstr_count_char_not ("Hello, World!", 'x'), 13);
	e_test_assert_eq ("e_cstr_count_char_not nul", usize, e_cstr_count_char_not ("Hello, World!", '\0'), 13);
	e_test_assert_eq ("e_cstr_count_char_not empty", usize, e_cstr_count_char_not ("", 'x'), 0);
	e_test_assert_eq ("e_cstr_count_char_not empty nul", usize, e_cstr_count_char_not ("", '\0'), 0);
	e_test_assert_eq ("e_cstr_count_char_not all", usize, e_cstr_count_char_not ("xxx", 'x'), 0);

	e_test_assert_eq ("e_cstr_count_char_pat", usize, e_cstr_count_char_pat ("Hello, World!", "load"), 6);
	e_test_assert_eq ("e_cstr_count_char_pat none", usize, e_cstr_count_char_pat ("Hello, World!", "xyz"), 0);
	e_test_assert_eq ("e_cstr_count_char_pat empty", usize, e_cstr_count_char_pat ("", "xyz"), 0);
	e_test_assert_eq ("e_cstr_count_char_pat all", usize, e_cstr_count_char_pat ("xxyyzz", "xyz"), 6);
	e_test_assert_eq ("e_cstr_count_char_pat empty pattern", usize, e_cstr_count_char_pat ("Hello, World!", ""), 0);

	e_test_assert_eq ("e_cstr_count_char_not_pat", usize, e_cstr_count_char_not_pat ("Hello, World!", "load"), 7);
	e_test_assert_eq ("e_cstr_count_char_not_pat none", usize, e_cstr_count_char_not_pat ("Hello, World!", "xyz"), 13);
	e_test_assert_eq ("e_cstr_count_char_not_pat empty", usize, e_cstr_count_char_not_pat ("", "xyz"), 0);
	e_test_assert_eq ("e_cstr_count_char_not_pat all", usize, e_cstr_count_char_not_pat ("xxyyzz", "xyz"), 0);
	e_test_assert_eq ("e_cstr_count_char_not_pat empty pattern", usize, e_cstr_count_char_not_pat ("Hello, World!", ""), 13);

	e_test_assert_eq ("e_cstr_count_char_func", usize, e_cstr_count_char_func ("Hello, World!", islower), 8);
	e_test_assert_eq ("e_cstr_count_char_func none", usize, e_cstr_count_char_func ("HELLO, WORLD!", islower), 0);
	e_test_assert_eq ("e_cstr_count_char_func empty", usize, e_cstr_count_char_func ("", islower), 0);
	e_test_assert_eq ("e_cstr_count_char_func all", usize, e_cstr_count_char_func ("helloworld", islower), 10);

	e_test_assert_eq ("e_cstr_count_char_not_func", usize, e_cstr_count_char_not_func ("Hello, World!", islower), 5);
	e_test_assert_eq ("e_cstr_count_char_not_func none", usize, e_cstr_count_char_not_func ("helloworld", islower), 0);
	e_test_assert_eq ("e_cstr_count_char_not_func empty", usize, e_cstr_count_char_not_func ("", islower), 0);
	e_test_assert_eq ("e_cstr_count_char_not_func all", usize, e_cstr_count_char_not_func ("HELLO, WORLD!", islower), 13);

	e_test_assert_eq ("e_cstr_count_str", usize, e_cstr_count_str ("fooXXbarXXXXbazXX", "XX"), 4);
	e_test_assert_eq ("e_cstr_count_str none", usize, e_cstr_count_str ("Hello, World!", "XX"), 0);
	e_test_assert_eq ("e_cstr_count_str empty", usize, e_cstr_count_str ("", "XX"), 0);
	e_test_assert_eq ("e_cstr_count_str all", usize, e_cstr_count_str ("XXXXXX", "XX"), 3);
	e_test_assert_eq ("e_cstr_count_str single", usize, e_cstr_count_str ("XX", "XX"), 1);
	e_test_assert_eq ("e_cstr_count_str overlap", usize, e_cstr_count_str ("XXXXX", "XX"), 2);
	e_test_assert_eq ("e_cstr_count_str too long", usize, e_cstr_count_str ("foo", "123456"), 0);
	e_test_assert_eq ("e_cstr_count_str empty needle", usize, e_cstr_count_str ("Hello, World!", ""), 13);

	e_test_assert_eq ("e_cstr_count_str_overlap", usize, e_cstr_count_str_overlap ("fooXXbarXXXXbazXX", "XX"), 5);
	e_test_assert_eq ("e_cstr_count_str_overlap none", usize, e_cstr_count_str_overlap ("Hello, World!", "XX"), 0);
	e_test_assert_eq ("e_cstr_count_str_overlap empty", usize, e_cstr_count_str_overlap ("", "XX"), 0);
	e_test_assert_eq ("e_cstr_count_str_overlap single", usize, e_cstr_count_str_overlap ("XX", "XX"), 1);
	e_test_assert_eq ("e_cstr_count_str_overlap overlap", usize, e_cstr_count_str_overlap ("XXXXX", "XX"), 4);
	e_test_assert_eq ("e_cstr_count_str_overlap too long", usize, e_cstr_count_str_overlap ("foo", "123456"), 0);
	e_test_assert_eq ("e_cstr_count_str_overlap empty needle", usize, e_cstr_count_str_overlap ("Hello, World!", ""), 13);

	e_test_assert_eq ("e_cstr_count_lines empty", usize, e_cstr_count_lines (""), 1);
	e_test_assert_eq ("e_cstr_count_lines one", usize, e_cstr_count_lines ("Hello, World!"), 1);
	e_test_assert_eq ("e_cstr_count_lines two", usize, e_cstr_count_lines ("foo \n\t \f \v  bar  \r "), 2);
}

static void
test_cstr_len (void)
{
	e_test_assert_eq ("e_cstr_len", usize, e_cstr_len ("Hello, World!"), 13);
	e_test_assert_eq ("e_cstr_len null", usize, e_cstr_len (nullptr), 0);
}

static void
test_cstr_eq (void)
{
	char s1[] = "Hello, World!";
	char s2[] = "Hello, World!";
	char s3[] = "Different";

	e_test_assert ("e_cstr_eq null null", e_cstr_eq (nullptr, nullptr));
	e_test_assert ("e_cstr_eq ptr null", !e_cstr_eq (s1, nullptr));
	e_test_assert ("e_cstr_eq ptr eq ptr", e_cstr_eq (s1, s1));
	e_test_assert ("e_cstr_eq eq", e_cstr_eq (s1, s2));
	e_test_assert ("e_cstr_eq not eq", !e_cstr_eq (s1, s3));

	e_test_assert ("e_cstr_starts_with true", e_cstr_starts_with (s1, "Hel"));
	e_test_assert ("e_cstr_starts_with false", !e_cstr_starts_with (s1, "foo"));
	e_test_assert ("e_cstr_starts_with empty", !e_cstr_starts_with ("", "foo"));
	e_test_assert ("e_cstr_starts_with empty needle", e_cstr_starts_with (s1, ""));

	e_test_assert ("e_cstr_ends_with true", e_cstr_ends_with (s1, "World!"));
	e_test_assert ("e_cstr_ends_with false", !e_cstr_starts_with (s1, "foo"));
	e_test_assert ("e_cstr_ends_with empty", !e_cstr_ends_with ("", "foo"));
	e_test_assert ("e_cstr_ends_with empty needle", e_cstr_ends_with (s1, ""));

	e_test_assert ("e_cstr_continues_with true", e_cstr_continues_with (s1, "lo,", 3));
	e_test_assert ("e_cstr_continues_with false", !e_cstr_continues_with (s1, "foo", 3));
	e_test_assert ("e_cstr_continues_with empty needle", e_cstr_continues_with (s1, "", 3));
}

static void
test_cstr_matches (void)
{
	e_test_assert ("e_cstr_is_blank blank", e_cstr_is_blank (" \n\t \f \v    \r "));
	e_test_assert ("e_cstr_is_blank non-blank", !e_cstr_is_blank ("Hello, World!"));
	e_test_assert ("e_cstr_is_blank empty", e_cstr_is_blank (""));

	e_test_assert ("e_cstr_is_ascii non-ascii", !e_cstr_is_ascii ("Größe"));
	e_test_assert ("e_cstr_is_ascii ascii", e_cstr_is_ascii ("Groesse"));
	e_test_assert ("e_cstr_is_ascii empty", e_cstr_is_ascii (""));

	e_test_assert ("e_cstr_matches_predicate false", !e_cstr_matches_predicate ("1337", islower));
	e_test_assert ("e_cstr_matches_predicate true", e_cstr_matches_predicate ("1337", isdigit));
	e_test_assert ("e_cstr_matches_predicate empty", e_cstr_matches_predicate ("", isdigit));
}

static void
test_cstr_find (void)
{
	const char *hello = "Hello, World!";
	const char *xxx = "xxx";
	const char *nums = "1337";
	const char *empty = "";
	const char *foo = "  foo   ";
	const char *sentence = "Dosenöffner öffnen Dosen.";

	e_test_assert_ptr_eq ("e_cstr_find_char existant", e_cstr_find_char (hello, 'o'), hello + 4);
	e_test_assert_null ("e_cstr_find_char nonexistant", e_cstr_find_char (hello, 'x'));
	e_test_assert_null ("e_cstr_find_char empty", e_cstr_find_char (empty, 'x'));
	e_test_assert_ptr_eq ("e_cstr_find_char nul", e_cstr_find_char (hello, '\0'), hello + 13);

	e_test_assert_ptr_eq ("e_cstr_find_char_not existant", e_cstr_find_char_not (hello, 'H'), hello + 1);
	e_test_assert_null ("e_cstr_find_char_not nonexistant", e_cstr_find_char_not (xxx, 'x'));
	e_test_assert_null ("e_cstr_find_char_not empty", e_cstr_find_char_not (empty, 'x'));
	e_test_assert_ptr_eq ("e_cstr_find_char_not nul", e_cstr_find_char_not (hello, '\0'), hello);

	e_test_assert_ptr_eq ("e_cstr_find_char_pat existant", e_cstr_find_char_pat (hello, "abcd"), hello + 11);
	e_test_assert_null ("e_cstr_find_char_pat nonexistant", e_cstr_find_char_pat (hello, "xyz"));
	e_test_assert_null ("e_cstr_find_char_pat empty", e_cstr_find_char_pat (empty, "xyz"));
	e_test_assert_null ("e_cstr_find_char_pat empty pattern", e_cstr_find_char_pat (hello, ""));

	e_test_assert_ptr_eq ("e_cstr_find_char_not_pat existant", e_cstr_find_char_not_pat (hello, "HWelo, "), hello + 9);
	e_test_assert_null ("e_cstr_find_char_not_pat nonexistant", e_cstr_find_char_not_pat (nums, "1234567890"));
	e_test_assert_null ("e_cstr_find_char_not_pat empty", e_cstr_find_char_not_pat (empty, "HWelo, "));
	e_test_assert_ptr_eq ("e_cstr_find_char_not_pat empty pattern", e_cstr_find_char_not_pat (hello, ""), hello);

	e_test_assert_ptr_eq ("e_cstr_find_char_func existant", e_cstr_find_char_func (foo, isalpha), foo + 2);
	e_test_assert_null ("e_cstr_find_char_func nonexistant", e_cstr_find_char_func (foo, isdigit));
	e_test_assert_null ("e_cstr_find_char_func empty", e_cstr_find_char_func (empty, isdigit));

	e_test_assert_ptr_eq ("e_cstr_find_char_not_func existant", e_cstr_find_char_not_func (foo, isspace), foo + 2);
	e_test_assert_null ("e_cstr_find_char_not_func nonexistant", e_cstr_find_char_not_func (nums, isdigit));
	e_test_assert_null ("e_cstr_find_char_not_func empty", e_cstr_find_char_not_func (empty, isdigit));

	e_test_assert_ptr_eq ("e_cstr_find_str existant", e_cstr_find_str (sentence, "en"), sentence + 3);
	e_test_assert_null ("e_cstr_find_str nonexistant", e_cstr_find_str (foo, "es"));
	e_test_assert_null ("e_cstr_find_str empty", e_cstr_find_str (empty, "foo"));
	e_test_assert_ptr_eq ("e_cstr_find_str empty needle", e_cstr_find_str (foo, ""), foo);

	e_test_assert_ptr_eq ("e_cstr_rfind_char existant", e_cstr_rfind_char (hello, 'o'), hello + 8);
	e_test_assert_null ("e_cstr_rfind_char nonexistant", e_cstr_rfind_char (hello, 'x'));
	e_test_assert_null ("e_cstr_rfind_char empty", e_cstr_rfind_char (empty, 'x'));
	e_test_assert_ptr_eq ("e_cstr_rfind_char nul", e_cstr_rfind_char (hello, '\0'), hello + 13);

	e_test_assert_ptr_eq ("e_cstr_rfind_char_not existant", e_cstr_rfind_char_not (hello, '!'), hello + 11);
	e_test_assert_null ("e_cstr_rfind_char_not nonexistant", e_cstr_rfind_char_not (xxx, 'x'));
	e_test_assert_null ("e_cstr_rfind_char_not empty", e_cstr_rfind_char_not (empty, 'x'));
	e_test_assert_ptr_eq ("e_cstr_rfind_char_not nul", e_cstr_rfind_char_not (hello, '\0'), hello + 12);

	e_test_assert_ptr_eq ("e_cstr_rfind_char_pat existant", e_cstr_rfind_char_pat (hello, "elo"), hello + 10);
	e_test_assert_null ("e_cstr_rfind_char_pat nonexistant", e_cstr_rfind_char_pat (hello, "xyz"));
	e_test_assert_null ("e_cstr_rfind_char_pat empty", e_cstr_rfind_char_pat (empty, "xyz"));
	e_test_assert_null ("e_cstr_rfind_char_pat empty pattern", e_cstr_rfind_char_pat (hello, ""));

	e_test_assert_ptr_eq ("e_cstr_rfind_char_not_pat existant", e_cstr_rfind_char_not_pat (hello, "du!"), hello + 10);
	e_test_assert_null ("e_cstr_rfind_char_not_pat nonexistant", e_cstr_rfind_char_not_pat (nums, "1234567890"));
	e_test_assert_null ("e_cstr_rfind_char_not_pat empty", e_cstr_rfind_char_not_pat (empty, "1234567890"));
	e_test_assert_ptr_eq ("e_cstr_rfind_char_not_pat empty pattern", e_cstr_rfind_char_not_pat (hello, ""), hello + 12);

	e_test_assert_ptr_eq ("e_cstr_rfind_char_func existant", e_cstr_rfind_char_func (foo, isalpha), foo + 4);
	e_test_assert_null ("e_cstr_rfind_char_func nonexistant", e_cstr_rfind_char_func (foo, isdigit));
	e_test_assert_null ("e_cstr_rfind_char_func empty", e_cstr_rfind_char_func (empty, isdigit));

	e_test_assert_ptr_eq ("e_cstr_rfind_char_not_func existant", e_cstr_rfind_char_not_func (foo, isspace), foo + 4);
	e_test_assert_null ("e_cstr_rfind_char_not_func nonexistant", e_cstr_rfind_char_not_func (nums, isdigit));
	e_test_assert_null ("e_cstr_rfind_char_not_func empty", e_cstr_rfind_char_not_func (empty, isdigit));

	e_test_assert_ptr_eq ("e_cstr_rfind_str existant", e_cstr_rfind_str (sentence, "en"), sentence + 24);
	e_test_assert_null ("e_cstr_rfind_str nonexistant", e_cstr_rfind_str (foo, "es"));
	e_test_assert_null ("e_cstr_rfind_str empty", e_cstr_rfind_str (empty, "foo"));
	e_test_assert_ptr_eq ("e_cstr_rfind_str empty needle", e_cstr_rfind_str (hello, ""), hello + 12);
}

static void
test_cstr_trim (void)
{
	const char *str = "  foo   ";
	const char *notrim = "foo";
	const char *empty = "";
	usize len;

	e_test_assert_ptr_eq ("e_cstr_trim_start", e_cstr_trim_start (str), str + 2);
	e_test_assert_ptr_eq ("e_cstr_trim_start notrim", e_cstr_trim_start (notrim), notrim);
	e_test_assert_ptr_eq ("e_cstr_trim_start empty", e_cstr_trim_start (empty), empty);

	e_test_assert_eq ("e_cstr_trim_end", usize, e_cstr_trim_end (str), 5);
	e_test_assert_eq ("e_cstr_trim_end notrim", usize, e_cstr_trim_end (notrim), 3);
	e_test_assert_eq ("e_cstr_trim_end empty", usize, e_cstr_trim_end (empty), 0);

	e_test_assert_ptr_eq ("e_cstr_trim", e_cstr_trim (str, &len), str + 2);
	e_test_assert_eq ("e_cstr_trim len", usize, len, 3);
	e_test_assert_ptr_eq ("e_cstr_trim notrim", e_cstr_trim (notrim, &len), notrim);
	e_test_assert_eq ("e_cstr_trim notrim len", usize, len, 3);
	e_test_assert_ptr_eq ("e_cstr_trim empty", e_cstr_trim (empty, &len), empty);
	e_test_assert_eq ("e_cstr_trim empty len", usize, len, 0);
}

static void
test_cstr_upper_lower (void)
{
	char s1[] = "Hello, World!";
	char s2[] = "Hello, World!";

	e_test_assert_str_eq ("e_cstr_to_ascii_lower", e_cstr_to_ascii_lower (s1), "hello, world!");
	e_test_assert_str_eq ("e_cstr_to_ascii_upper", e_cstr_to_ascii_upper (s2), "HELLO, WORLD!");
}

static void
test_cstr_distance (void)
{
	e_test_assert_eq ("e_cstr_distance eq", usize, e_cstr_distance ("kitten", "kitten"), 0);
	e_test_assert_eq ("e_cstr_distance empty str", usize, e_cstr_distance ("", "kitten"), 6);
	e_test_assert_eq ("e_cstr_distance str empty", usize, e_cstr_distance ("kitten", ""), 6);
	e_test_assert_eq ("e_cstr_distance empty empty", usize, e_cstr_distance ("", ""), 0);
	e_test_assert_eq ("e_cstr_distance kitten sitting", usize, e_cstr_distance ("kitten", "sitting"), 3);
	e_test_assert_eq ("e_cstr_distance kitten kittens", usize, e_cstr_distance ("kitten", "kittens"), 1);
	e_test_assert_eq ("e_cstr_distance kitten written", usize, e_cstr_distance ("kitten", "written"), 2);
	e_test_assert_eq ("e_cstr_distance kitten bitten", usize, e_cstr_distance ("kitten", "bitten"), 1);
	e_test_assert_eq ("e_cstr_distance kitten kitte", usize, e_cstr_distance ("kitten", "kitte"), 1);
	e_test_assert_eq ("e_cstr_distance kitten kitty", usize, e_cstr_distance ("kitten", "kitty"), 2);
	e_test_assert_eq ("e_cstr_distance kitten kite", usize, e_cstr_distance ("kitten", "kite"), 2);
	e_test_assert_eq ("e_cstr_distance kitten bite", usize, e_cstr_distance ("kitten", "bite"), 3);
	e_test_assert_eq ("e_cstr_distance kitten absurdly", usize, e_cstr_distance ("kitten", "absurdly"), 8);
}

static void
test_cstr_split (void)
{
	E_Str_Split split;

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
