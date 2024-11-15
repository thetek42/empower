#include <ctype.h>

#define E_ALLOC_IMPL
#define E_BASE64_IMPL
#define E_CSTR_IMPL
#define E_DEBUG_IMPL
#define E_FS_IMPL
#define E_INI_IMPL
#define E_LOG_IMPL
#define E_MEM_IMPL
#define E_PARSE_IMPL
#define E_RAND_IMPL
#define E_RESULT_IMPL
#define E_STR_IMPL
#define E_TEST_IMPL

#include <e_alloc.h>
#include <e_base64.h>
#include <e_cstr.h>
#include <e_debug.h>
#include <e_fs.h>
#include <e_ini.h>
#include <e_log.h>
#include <e_macro.h>
#include <e_mem.h>
#include <e_parse.h>
#include <e_rand.h>
#include <e_result.h>
#include <e_str.h>
#include <e_test.h>
#include <e_types.h>
#include <e_vec.h>

E_VEC_DECL (int, E_Vec_Int, e_vec_int);
E_VEC_IMPL (int, E_Vec_Int, e_vec_int);

static void test_base64 (void);
static void test_cstr (void);
static void test_fs (void);
static void test_ini (void);
static void test_parse (void);
static void test_str (void);
static void test_vec (void);

int
main (int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	test_base64 ();
	test_cstr ();
	test_fs ();
	test_ini ();
	test_parse ();
	test_str ();
	test_vec ();

	e_test_finish ();
}

static void
test_base64 (void)
{
	char *plain = "Many hands make light work.";
	char *encoded, *plain_out;

	encoded = e_base64_enc_alloc (plain);
	plain_out = e_base64_dec_alloc (encoded);

	e_test_assert_str_eq ("e_base64_enc", encoded, "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");
	e_test_assert_str_eq ("e_base64_dec", plain_out, plain);

	e_free (encoded);
	e_free (plain_out);
}

static void
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

static void
test_fs (void)
{
	E_File file;
	char *buf, buf2[15];
	usize len;

	e_test_assert_ok ("e_fs_file_open write", e_fs_file_open (&file, "test/test.txt", E_FS_OPEN_MODE_WRITE_TRUNC));
	e_test_assert_ok ("e_fs_file_write", e_fs_file_write (&file, "Hello, World!\t", strlen ("Hello, World!\t")));
	e_test_assert_ok ("e_fs_file_write_fmt", e_fs_file_write_fmt (&file, &len, "%d", 42));
	e_test_assert_eq ("e_fs_file_write_fmt len", usize, len, 2);
	e_fs_file_close (&file);

	e_test_assert_ok ("e_fs_file_open read", e_fs_file_open (&file, "test/test.txt", E_FS_OPEN_MODE_READ_ONLY));

	e_test_assert_ok ("e_fs_file_get_size", e_fs_file_get_size (&file, &len));
	e_test_assert_eq ("e_fs_file_get_size len", usize, len, strlen ("Hello, World!\t42"));
	e_test_assert_ok ("e_fs_file_read 1", e_fs_file_read (&file, buf2, 15, &len));
	e_test_assert_eq ("e_fs_file_read 1 len", usize, len, strlen ("Hello, World!\t"));
	e_test_assert_str_eq ("e_fs_file_read 1 out", buf2, "Hello, World!\t");
	e_test_assert_ok ("e_fs_file_get_remaining_size", e_fs_file_get_remaining_size (&file, &len));
	e_test_assert_eq ("e_fs_file_get_remaining_size len", usize, len, 2);
	e_test_assert_ok ("e_fs_file_get_pos 1", e_fs_file_get_pos (&file, &len));
	e_test_assert_eq ("e_fs_file_get_pos 1 out", usize, len, strlen ("Hello, World!\t"));
	e_test_assert_ok ("e_fs_file_set_pos", e_fs_file_set_pos (&file, len - 2));
	e_test_assert_ok ("e_fs_file_get_pos 2", e_fs_file_get_pos (&file, &len));
	e_test_assert_eq ("e_fs_file_get_pos 2 out", usize, len, strlen ("Hello, World"));
	e_test_assert_ok ("e_fs_file_set_pos_end", e_fs_file_set_pos_end (&file, 7));
	e_test_assert_ok ("e_fs_file_get_pos 3", e_fs_file_get_pos (&file, &len));
	e_test_assert_eq ("e_fs_file_get_pos 3 out", usize, len, strlen ("Hello, Wo"));
	e_test_assert_ok ("e_fs_file_read 2", e_fs_file_read (&file, buf2, 15, &len));
	e_test_assert_eq ("e_fs_file_read 2 len", usize, len, strlen ("rld!\t42"));
	e_test_assert_str_eq ("e_fs_file_read 2 out", buf2, "rld!\t42");
	e_test_assert_ok ("e_fs_file_rewind", e_fs_file_rewind (&file));
	e_test_assert_ok ("e_fs_file_read 3", e_fs_file_read (&file, buf2, 8, &len));
	e_test_assert_eq ("e_fs_file_read 3 len", usize, len, strlen ("Hello, "));

	e_test_assert_str_eq ("e_fs_file_read 3 out", buf2, "Hello, ");
	e_test_assert_ok ("e_fs_file_read_remaining", e_fs_file_read_remaining (&file, &buf, &len));
	e_test_assert_str_eq ("e_fs_file_read_remaining out", buf, "World!\t42");
	e_test_assert_eq ("e_fs_file_read_remaining len", usize, len, strlen ("World!\t42"));
	e_free (buf);

	e_test_assert_ok ("e_fs_file_read_all", e_fs_file_read_all (&file, &buf, &len));
	e_test_assert_str_eq ("e_fs_file_read_all out", buf, "Hello, World!\t42");
	e_test_assert_eq ("e_fs_file_read_all len", usize, len, strlen ("Hello, World!\t42"));
	e_free (buf);
	e_fs_file_close (&file);

	e_test_assert ("e_fs_path_exists true", e_fs_path_exists ("test/test.txt"));
	e_test_assert ("e_fs_path_exists false", !e_fs_path_exists ("eierschale.docx"));
	e_test_assert ("e_fs_is_file true", e_fs_is_file ("test/test.txt"));
	e_test_assert ("e_fs_is_file false", !e_fs_is_file ("test"));
	e_test_assert ("e_fs_is_dir true", e_fs_is_dir ("test"));
	e_test_assert ("e_fs_is_dir false", !e_fs_is_dir ("test/test.txt"));
}

static void
test_ini (void)
{
	E_Ini ini;

	const char *ini_str =
		"no_section = 123\n"
		"\n"
		"; comment\n"
		"[foo]\n"
		"  bar = baz \n"
		"qux =\n"
		"\n"
		"  [bar]  ; comment\n"
		"rab = zab\n";

	e_test_assert_ok ("e_ini_parse_str", e_ini_parse_str (&ini, ini_str));
	e_test_assert_str_eq ("e_ini_get_entry no_section", e_ini_get_entry (&ini, nullptr, "no_section")->value, "123");
	e_test_assert_str_eq ("e_ini_get_entry section", e_ini_get_entry (&ini, "bar", "rab")->value, "zab");
	e_test_assert_str_eq ("e_ini_get_entry empty", e_ini_get_entry (&ini, "foo", "qux")->value, "");
	e_test_assert_null ("e_ini_get_entry invalid section", e_ini_get_entry (&ini, "kjfadsf", "foo"));
	e_test_assert_null ("e_ini_get_entry invalid key", e_ini_get_entry (&ini, "foo", "kjfadsf"));
	e_ini_deinit (&ini);
}

static void
test_parse (void)
{
	const char *orig = "int foo (void) {\n    return 42; \n}";
	const char *s;

	s = orig;
	/* s = "int foo (void) {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_char 1 ret", e_parse_char (&s, 'i'), orig);
	e_test_assert_ptr_eq ("e_parse_char 1 ptr", s, orig + 1);
	/* s = "nt foo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char 2 ret", e_parse_char (&s, 'i'));
	e_test_assert_ptr_eq ("e_parse_char 2 ptr", s, orig + 1);
	e_test_assert_ptr_eq ("e_parse_char_not 1 ret", e_parse_char_not (&s, 't'), orig + 1);
	e_test_assert_ptr_eq ("e_parse_char_not 1 ptr", s, orig + 2);
	/* s = "t foo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_not 2 ret", e_parse_char_not (&s, 't'));
	e_test_assert_ptr_eq ("e_parse_char_not 2 ptr", s, orig + 2);
	e_test_assert_ptr_eq ("e_parse_char_pat 1 ret", e_parse_char_pat (&s, "ajtbe"), orig + 2);
	e_test_assert_ptr_eq ("e_parse_char_pat 1 ptr", s, orig + 3);
	/* s = " foo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_pat 2 ret", e_parse_char_pat (&s, "ajtbe"));
	e_test_assert_ptr_eq ("e_parse_char_pat 2 ptr", s, orig + 3);
	e_test_assert_ptr_eq ("e_parse_char_not_pat 1 ret", e_parse_char_not_pat (&s, "abcdefg"), orig + 3);
	e_test_assert_ptr_eq ("e_parse_char_not_pat 1 ptr", s, orig + 4);
	/* s = "foo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_not_pat 2 ret", e_parse_char_not_pat (&s, "abcdefg"));
	e_test_assert_ptr_eq ("e_parse_char_not_pat 2 ptr", s, orig + 4);
	e_test_assert_ptr_eq ("e_parse_char_func 1 ret", e_parse_char_func (&s, isalpha), orig + 4);
	e_test_assert_ptr_eq ("e_parse_char_func 1 ptr", s, orig + 5);
	/* s = "oo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_func 2 ret", e_parse_char_func (&s, isdigit));
	e_test_assert_ptr_eq ("e_parse_char_func 2 ptr", s, orig + 5);
	e_test_assert_ptr_eq ("e_parse_char_not_pat 1 ret", e_parse_char_not_func (&s, isdigit), orig + 5);
	e_test_assert_ptr_eq ("e_parse_char_not_pat 1 ptr", s, orig + 6);
	/* s = "o (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_not_pat 2 ret", e_parse_char_not_func (&s, isalpha));
	e_test_assert_ptr_eq ("e_parse_char_not_pat 2 ptr", s, orig + 6);

	s = orig;
	/* s = "int foo (void) {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_str_match 1 ret", e_parse_str_match (&s, "int f"), orig);
	e_test_assert_ptr_eq ("e_parse_str_match 1 ptr", s, orig + 5);
	/* s = "oo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_str_match 2 ret", e_parse_str_match (&s, "int f"));
	e_test_assert_ptr_eq ("e_parse_str_match 2 ptr", s, orig + 5);
	e_test_assert_ptr_eq ("e_parse_consume_while_char ret", e_parse_consume_while_char (&s, 'o'), orig + 5);
	e_test_assert_ptr_eq ("e_parse_consume_while_char ptr", s, orig + 7);
	/* s = " (void) {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_while_pat ret", e_parse_consume_while_pat (&s, "(v )"), orig + 7);
	e_test_assert_ptr_eq ("e_parse_consume_while_pat ptr", s, orig + 10);
	/* s = "oid) {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_while_func ret", e_parse_consume_while_func (&s, isalpha), orig + 10);
	e_test_assert_ptr_eq ("e_parse_consume_while_func ptr", s, orig + 13);
	/* s = ") {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_until_char ret", e_parse_consume_until_char (&s, '{'), orig + 13);
	e_test_assert_ptr_eq ("e_parse_consume_until_char ptr", s, orig + 15);
	/* s = "{\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_until_pat ret", e_parse_consume_until_pat (&s, "ert"), orig + 15);
	e_test_assert_ptr_eq ("e_parse_consume_until_pat ptr", s, orig + 21);
	/* s = "return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_until_func ret", e_parse_consume_until_func (&s, isspace), orig + 21);
	e_test_assert_ptr_eq ("e_parse_consume_until_func ptr", s, orig + 27);
	/* s = " 42; \n} */

	s = orig + 16;
	/* s = "\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_whitespace ret", e_parse_consume_whitespace (&s), orig + 16);
	e_test_assert_ptr_eq ("e_parse_consume_whitespace ptr", s, orig + 21);
	/* s = "return 42; \n} */
	s += 10;
	/* s = " \n} */
	e_test_assert_ptr_eq ("e_parse_consume_whitespace_until_newline ret", e_parse_consume_whitespace_until_newline (&s), orig + 31);
	e_test_assert_ptr_eq ("e_parse_consume_whitespace_until_newline ptr", s, orig + 32);
	e_test_assert_ptr_eq ("e_parse_char \\n ret", e_parse_char (&s, '\n'), orig + 32);
	e_test_assert_ptr_eq ("e_parse_char \\n ptr", s, orig + 33);
}

static void
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

static void
test_vec (void)
{
	E_Vec_Int vec;
	int slice[] = { 4, 5, 6, 7, 8 };
	int slice2[] = { 3, 4 };
	int slice3[] = { 4, 3 };
	int slice4[] = { 2, 3 };
	int slice5[] = { 4, 4 };
	int *popped;
	usize len;

	/* [] */
	vec = e_vec_int_init ();
	e_test_assert_null ("e_vec_init ptr", vec.ptr);
	e_test_assert_eq ("e_vec_init len", usize, vec.len, 0);
	e_test_assert_eq ("e_vec_init cap", usize, vec.cap, 0);
	e_test_assert_null ("e_vec_first empty", e_vec_int_get_first (&vec));
	e_test_assert_null ("e_vec_last empty", e_vec_int_get_last (&vec));

	/* [1] */
	e_vec_int_append (&vec, 1);
	e_test_assert_eq ("e_vec_append 1 ptr[0]", int, vec.ptr[0], 1);
	e_test_assert_eq ("e_vec_append 1 len", usize, vec.len, 1);
	e_test_assert_eq ("e_vec_append 1 cap", usize, vec.cap, 1);

	/* [1,2] */
	e_vec_int_append (&vec, 2);
	e_test_assert_eq ("e_vec_append 2 ptr[1]", int, vec.ptr[1], 2);
	e_test_assert_eq ("e_vec_append 2 len", usize, vec.len, 2);
	e_test_assert_eq ("e_vec_append 2 cap", usize, vec.cap, 2);

	/* [1,2,3] */
	e_vec_int_append (&vec, 3);
	e_test_assert_eq ("e_vec_append 3 ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_append 3 len", usize, vec.len, 3);
	e_test_assert_eq ("e_vec_append 3 cap", usize, vec.cap, 4);

	/* [1,2,3,4,5,6,7,8] */
	e_vec_int_append_slice (&vec, slice, 5);
	e_test_assert_eq ("e_vec_append_slice ptr[3]", int, vec.ptr[3], 4);
	e_test_assert_eq ("e_vec_append_slice ptr[7]", int, vec.ptr[7], 8);
	e_test_assert_eq ("e_vec_append_slice len", usize, vec.len, 8);
	e_test_assert_eq ("e_vec_append_slice cap", usize, vec.cap, 8);

	e_test_assert_ptr_eq ("e_vec_find existing", e_vec_int_find (&vec, 4), &vec.ptr[3]);
	e_test_assert_null ("e_vec_find nonexisting", e_vec_int_find (&vec, 42));
	e_test_assert_eq ("e_vec_find_idx existing", isize, e_vec_int_find_idx (&vec, 4), 3);
	e_test_assert_eq ("e_vec_find_idx nonexisting", isize, e_vec_int_find_idx (&vec, 42), -1);
	e_test_assert_ptr_eq ("e_vec_find_slice existing", e_vec_int_find_slice (&vec, slice2, 2), &vec.ptr[2]);
	e_test_assert_null ("e_vec_find_slice nonexisting", e_vec_int_find_slice (&vec, slice3, 2));
	e_test_assert_eq ("e_vec_find_slice_idx existing", isize, e_vec_int_find_slice_idx (&vec, slice2, 2), 2);
	e_test_assert_eq ("e_vec_find_slice_idx nonexisting", isize, e_vec_int_find_slice_idx (&vec, slice3, 2), -1);
	e_test_assert ("e_vec_contains true", e_vec_int_contains (&vec, 4));
	e_test_assert ("e_vec_contains false", !e_vec_int_contains (&vec, 42));
	e_test_assert ("e_vec_contains_slice true", e_vec_int_contains_slice (&vec, slice2, 2));
	e_test_assert ("e_vec_contains_slice false", !e_vec_int_contains_slice (&vec, slice3, 2));
	e_test_assert_ptr_eq ("e_vec_get existing", e_vec_int_get (&vec, 4), &vec.ptr[4]);
	e_test_assert_null ("e_vec_get nonexisting", e_vec_int_get (&vec, 42));
	e_test_assert_ptr_eq ("e_vec_get_back existing", e_vec_int_get_back (&vec, 2), &vec.ptr[5]);
	e_test_assert_null ("e_vec_get_back nonexisting", e_vec_int_get_back (&vec, 42));
	e_test_assert_ptr_eq ("e_vec_first", e_vec_int_get_first (&vec), &vec.ptr[0]);
	e_test_assert_ptr_eq ("e_vec_last", e_vec_int_get_last (&vec), &vec.ptr[7]);
	e_test_assert ("e_vec_set ok", e_vec_int_set (&vec, 1, 42));
	e_test_assert ("e_vec_set out of range", !e_vec_int_set (&vec, 42, 1));
	e_test_assert ("e_vec_set_slice ok", e_vec_int_set_slice (&vec, 1, slice4, 2));
	e_test_assert ("e_vec_set_slice out of range", !e_vec_int_set_slice (&vec, 42, slice4, 2));

	/* [1,2,3,4,5,6,7] */
	e_vec_int_pop (&vec, &popped);
	e_test_assert_eq ("e_vec_pop 1 item", int, *popped, 8);
	e_test_assert_eq ("e_vec_pop 1 len", usize, vec.len, 7);
	e_test_assert_eq ("e_vec_pop 1 cap", usize, vec.cap, 8);

	/* [1,2,3,4,5,6] */
	e_vec_int_pop (&vec, &popped);
	e_test_assert_eq ("e_vec_pop 2 item", int, *popped, 7);
	e_test_assert_eq ("e_vec_pop 2 len", usize, vec.len, 6);
	e_test_assert_eq ("e_vec_pop 2 cap", usize, vec.cap, 8);

	/* [1,2,3,4,5,6,9] */
	e_vec_int_append (&vec, 9);
	e_test_assert_eq ("e_vec_append 4 ptr[6]", int, vec.ptr[6], 9);
	e_test_assert_eq ("e_vec_append 4 len", usize, vec.len, 7);
	e_test_assert_eq ("e_vec_append 4 cap", usize, vec.cap, 8);

	/* [1,2,3,11,4,5,6,9] */
	e_vec_int_insert (&vec, 3, 11);
	e_test_assert_eq ("e_vec_insert ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_insert ptr[3]", int, vec.ptr[3], 11);
	e_test_assert_eq ("e_vec_insert ptr[4]", int, vec.ptr[4], 4);
	e_test_assert_eq ("e_vec_insert len", usize, vec.len, 8);
	e_test_assert_eq ("e_vec_insert cap", usize, vec.cap, 8);

	/* [1,2,3,11,4,5,6,9,12] */
	e_vec_int_insert (&vec, 42, 12);
	e_test_assert_eq ("e_vec_insert append ptr[7]", int, vec.ptr[7], 9);
	e_test_assert_eq ("e_vec_insert append ptr[8]", int, vec.ptr[8], 12);
	e_test_assert_eq ("e_vec_insert append len", usize, vec.len, 9);
	e_test_assert_eq ("e_vec_insert append cap", usize, vec.cap, 16);

	/* [1,2,3,11,4,5,3,4,6,9,12] */
	e_vec_int_insert_slice (&vec, 6, slice2, 2);
	e_test_assert_eq ("e_vec_insert_slice ptr[5]", int, vec.ptr[5], 5);
	e_test_assert_eq ("e_vec_insert_slice ptr[6]", int, vec.ptr[6], 3);
	e_test_assert_eq ("e_vec_insert_slice ptr[7]", int, vec.ptr[7], 4);
	e_test_assert_eq ("e_vec_insert_slice ptr[8]", int, vec.ptr[8], 6);
	e_test_assert_eq ("e_vec_insert_slice len", usize, vec.len, 11);
	e_test_assert_eq ("e_vec_insert_slice cap", usize, vec.cap, 16);

	/* [1,2,3,11,4,5,3,4,6,9,12,3,4] */
	e_vec_int_insert_slice (&vec, 42, slice2, 2);
	e_test_assert_eq ("e_vec_insert_slice append ptr[10]", int, vec.ptr[10], 12);
	e_test_assert_eq ("e_vec_insert_slice append ptr[11]", int, vec.ptr[11], 3);
	e_test_assert_eq ("e_vec_insert_slice append ptr[12]", int, vec.ptr[12], 4);
	e_test_assert_eq ("e_vec_insert_slice append len", usize, vec.len, 13);
	e_test_assert_eq ("e_vec_insert_slice append cap", usize, vec.cap, 16);

	e_vec_int_append (&vec, 4);
	e_vec_int_append (&vec, 4);
	e_test_assert_eq ("e_vec_count", usize, e_vec_int_count (&vec, 3), 3);
	e_test_assert_eq ("e_vec_count_slice 1", usize, e_vec_int_count_slice (&vec, slice2, 2), 2);
	e_test_assert_eq ("e_vec_count_slice 2", usize, e_vec_int_count_slice (&vec, slice5, 2), 1);
	e_test_assert_eq ("e_vec_count_slice_overlap", usize, e_vec_int_count_slice_overlap (&vec, slice5, 2), 2);
	e_vec_int_pop_slice (&vec, nullptr, 2);

	/* [1,2,3,11,4,5,3,4,6] */
	len = e_vec_int_pop_slice (&vec, &popped, 4);
	e_test_assert_eq ("e_vec_pop_slice 1 item", int, *popped, 9);
	e_test_assert_eq ("e_vec_pop_slice 1 len", usize, vec.len, 9);
	e_test_assert_eq ("e_vec_pop_slice 1 cap", usize, vec.cap, 16);
	e_test_assert_eq ("e_vec_pop_slice 1 returned len", usize, len, 4);

	/* [1,2,3,11,4,5,3] */
	len = e_vec_int_remove (&vec, 7, 3);
	e_test_assert_eq ("e_vec_remove end ptr[6]", int, vec.ptr[6], 3);
	e_test_assert_eq ("e_vec_remove end len", usize, vec.len, 7);
	e_test_assert_eq ("e_vec_remove end cap", usize, vec.cap, 16);
	e_test_assert_eq ("e_vec_remove end ret", usize, len, 2);

	/* [1,2,3,5,3] */
	e_vec_int_remove (&vec, 3, 2);
	e_test_assert_eq ("e_vec_remove middle ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_remove middle ptr[3]", int, vec.ptr[3], 5);
	e_test_assert_eq ("e_vec_remove middle len", usize, vec.len, 5);
	e_test_assert_eq ("e_vec_remove middle cap", usize, vec.cap, 16);

	/* [] */
	len = e_vec_int_pop_slice (&vec, &popped, 8);
	e_test_assert_eq ("e_vec_pop_slice 2 item", int, *popped, 1);
	e_test_assert_eq ("e_vec_pop_slice 2 len", usize, vec.len, 0);
	e_test_assert_eq ("e_vec_pop_slice 2 cap", usize, vec.cap, 16);
	e_test_assert_eq ("e_vec_pop_slice 2 returned len", usize, len, 5);

	e_vec_int_deinit (&vec);

	vec = e_vec_int_repeat (42, 3);
	e_test_assert_eq ("e_vec_repeat ptr[0]", int, vec.ptr[0], 42);
	e_test_assert_eq ("e_vec_repeat ptr[1]", int, vec.ptr[1], 42);
	e_test_assert_eq ("e_vec_repeat ptr[2]", int, vec.ptr[2], 42);
	e_test_assert_eq ("e_vec_repeat len", usize, vec.len, 3);
	e_vec_int_deinit (&vec);

	vec = e_vec_int_repeat_slice (slice2, 2, 2);
	e_test_assert_eq ("e_vec_repeat ptr[0]", int, vec.ptr[0], 3);
	e_test_assert_eq ("e_vec_repeat ptr[1]", int, vec.ptr[1], 4);
	e_test_assert_eq ("e_vec_repeat ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_repeat ptr[3]", int, vec.ptr[3], 4);
	e_test_assert_eq ("e_vec_repeat len", usize, vec.len, 4);
	e_vec_int_deinit (&vec);
}
