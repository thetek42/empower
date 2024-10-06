/* for some reason, one must specify `-D__E_CONFIG_TEST_FORCE_TYPES` in the
 * .clangd file, otherwise clangd will complain about something. but if you do
 * that, clangd complains that you are redefining a macro, hence this otherwise
 * useless guard.*/
#ifndef __E_CONFIG_TEST_FORCE_TYPES
# define __E_CONFIG_TEST_FORCE_TYPES
#endif /* __E_CONFIG_TEST_FORCE_TYPES */

#include "empower.h"

E_VEC_DECLARE (int, E_Vec_Int, e_vec_int);
E_VEC_IMPLEMENT (int, E_Vec_Int, e_vec_int);

static void test_arena (void);
static void test_cstr (void);
static void test_enc (void);
static void test_fs (void);
static void test_ini (void);
static void test_parse (void);
static void test_str (void);
static void test_vec (void);

E_TEST_MAIN ();

int
main (int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	test_arena ();
	test_cstr ();
	test_enc ();
	test_fs ();
	test_ini ();
	test_parse ();
	test_str ();
	test_vec ();

	e_test_finish ();
}

static void
test_arena (void)
{
	E_Arena arena;
	void *ptr;

	arena = e_arena_init (16);

	ptr = e_arena_alloc (&arena, u32, 1);
	e_test_assert_ptr_eq ("e_arena_alloc ptr", ptr, arena.ptr);
	e_test_assert_ptr_aligned ("e_arena_alloc align", ptr, 4);
	e_test_assert_eq ("e_arena_alloc len", usize, arena.len, 4);

	ptr = e_arena_alloc_zero (&arena, u16, 1);
	e_test_assert_ptr_eq ("e_arena_alloc_zero ptr", ptr, arena.ptr + 4);
	e_test_assert_ptr_aligned ("e_arena_alloc_zero align", ptr, 2);
	e_test_assert_eq ("e_arena_alloc_zero len", usize, arena.len, 6);
	e_test_assert ("e_arena_alloc_zero mem", e_mem_is_zero (ptr, 2));

	ptr = e_arena_alloc_aligned (&arena, 8, 8);
	e_test_assert_ptr_eq ("e_arena_alloc_aligned ptr", ptr, arena.ptr + 8);
	e_test_assert_ptr_aligned ("e_arena_alloc_aligned align", ptr, 8);
	e_test_assert_eq ("e_arena_alloc_aligned len", usize, arena.len, 16);

	e_arena_deinit (&arena);
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
	usize len;

	e_test_assert_eq ("e_cstr_count_char", usize, e_cstr_count_char (s1, 'l'), 3);
	e_test_assert_eq ("e_cstr_count_char nul", usize, e_cstr_count_char (s1, '\0'), 0);
	e_test_assert_eq ("e_cstr_count_char_not", usize, e_cstr_count_char_not (s1, 'l'), strlen (s1) - 3);
	e_test_assert_eq ("e_cstr_count_char_pat", usize, e_cstr_count_char_pat (s1, "load"), 6);
	e_test_assert_eq ("e_cstr_count_char_not_pat", usize, e_cstr_count_char_not_pat (s1, "load"), strlen (s1) - 6);
	e_test_assert_eq ("e_cstr_count_char_func", usize, e_cstr_count_char_func (s1, islower), 8);
	e_test_assert_eq ("e_cstr_count_char_not_func", usize, e_cstr_count_char_not_func (s1, islower), strlen (s1) - 8);
	e_test_assert_eq ("e_cstr_count_str", usize, e_cstr_count_str (s4, "en"), 3);
	e_test_assert_eq ("e_cstr_count_str too long", usize, e_cstr_count_str (s3, "123456"), 0);
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
	e_test_assert ("e_cstr_eq null null", e_cstr_eq (nullptr, nullptr));
	e_test_assert ("e_cstr_eq ptr null", !e_cstr_eq (s5, nullptr));
	e_test_assert ("e_cstr_eq ptr eq ptr", e_cstr_eq (s5, s5));
	e_test_assert ("e_cstr_eq eq", e_cstr_eq (s5, s7));
	e_test_assert ("e_cstr_eq not eq", !e_cstr_eq (s5, s4));
	e_test_assert ("e_cstr_matches_predicate false", !e_cstr_matches_predicate (s3, islower));
	e_test_assert ("e_cstr_matches_predicate true", e_cstr_matches_predicate (s3, isdigit));
	e_test_assert_str_eq ("e_cstr_to_ascii_lower", e_cstr_to_ascii_lower (s1), "hello, world!");
	e_test_assert_str_eq ("e_cstr_to_ascii_upper", e_cstr_to_ascii_upper (s1), "HELLO, WORLD!");
	e_test_assert_ptr_eq ("e_cstr_trim_start", e_cstr_trim_start (s5), s5 + 2);
	e_test_assert_ptr_eq ("e_cstr_trim_start empty", e_cstr_trim_start (s6), s6);
	e_test_assert_eq ("e_cstr_trim_end", usize, e_cstr_trim_end (s5), strlen (s5) - 3);
	e_test_assert_eq ("e_cstr_trim_end empty", usize, e_cstr_trim_end (s6), 0);
	e_test_assert_ptr_eq ("e_cstr_trim", e_cstr_trim (s5, &len), s5 + 2);
	e_test_assert_eq ("e_cstr_trim len", usize, len, 3);
	e_test_assert_ptr_eq ("e_cstr_trim empty", e_cstr_trim (s6, &len), s6);
	e_test_assert_eq ("e_cstr_trim empty len", usize, len, 0);
}

static void
test_enc (void)
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

	len_ret = e_str_append_fmt (&str, 32, " dec:%d,hex:%x", 42, 42);
	e_test_assert_str_eq ("e_str_append_fmt", str.ptr, "Hello, World! 123 dec:42,hex:2a");
	e_test_assert_eq ("e_str_append_fmt len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a"));
	e_test_assert_eq ("e_str_append_fmt len_ret", usize, len_ret, strlen (" dec:42,hex:2a"));

	len_ret = e_str_append_fmt (&str, 4, " %d", 12345);
	e_test_assert_str_eq ("e_str_append_fmt capped", str.ptr, "Hello, World! 123 dec:42,hex:2a 123");
	e_test_assert_eq ("e_str_append_fmt capped len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a 123"));
	e_test_assert_eq ("e_str_append_fmt capped len_ret", usize, len_ret, 6);

	len_ret = e_str_append_fmt_all (&str, " %d", 12345);
	e_test_assert_str_eq ("e_str_append_fmt_all", str.ptr, "Hello, World! 123 dec:42,hex:2a 123 12345");
	e_test_assert_eq ("e_str_append_fmt_all len", usize, str.len, strlen ("Hello, World! 123 dec:42,hex:2a 123 12345"));
	e_test_assert_eq ("e_str_append_fmt_all len_ret", usize, len_ret, 6);

	e_str_deinit (&str);
}

static void
test_vec (void)
{
	E_Vec_Int vec;
	int slice[] = { 4, 5, 6, 7, 8 };
	int *popped;
	usize len;

	vec = e_vec_int_init ();
	e_test_assert_null ("e_vec_init ptr", vec.ptr);
	e_test_assert_eq ("e_vec_init len", usize, vec.len, 0);
	e_test_assert_eq ("e_vec_init cap", usize, vec.cap, 0);

	e_vec_int_append (&vec, 1);
	e_test_assert_eq ("e_vec_append 1 ptr[0]", int, vec.ptr[0], 1);
	e_test_assert_eq ("e_vec_append 1 len", usize, vec.len, 1);
	e_test_assert_eq ("e_vec_append 1 cap", usize, vec.cap, 1);

	e_vec_int_append (&vec, 2);
	e_test_assert_eq ("e_vec_append 2 ptr[1]", int, vec.ptr[1], 2);
	e_test_assert_eq ("e_vec_append 2 len", usize, vec.len, 2);
	e_test_assert_eq ("e_vec_append 2 cap", usize, vec.cap, 2);

	e_vec_int_append (&vec, 3);
	e_test_assert_eq ("e_vec_append 3 ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_append 3 len", usize, vec.len, 3);
	e_test_assert_eq ("e_vec_append 3 cap", usize, vec.cap, 4);

	e_vec_int_append_slice (&vec, slice, 5);
	e_test_assert_eq ("e_vec_append_slice ptr[3]", int, vec.ptr[3], 4);
	e_test_assert_eq ("e_vec_append_slice ptr[7]", int, vec.ptr[7], 8);
	e_test_assert_eq ("e_vec_append_slice len", usize, vec.len, 8);
	e_test_assert_eq ("e_vec_append_slice cap", usize, vec.cap, 8);

	e_vec_int_pop (&vec, &popped);
	e_test_assert_eq ("e_vec_pop 1 item", int, *popped, 8);
	e_test_assert_eq ("e_vec_pop 1 len", usize, vec.len, 7);
	e_test_assert_eq ("e_vec_pop 1 cap", usize, vec.cap, 8);

	e_vec_int_pop (&vec, &popped);
	e_test_assert_eq ("e_vec_pop 2 item", int, *popped, 7);
	e_test_assert_eq ("e_vec_pop 2 len", usize, vec.len, 6);
	e_test_assert_eq ("e_vec_pop 2 cap", usize, vec.cap, 8);

	e_vec_int_append (&vec, 9);
	e_test_assert_eq ("e_vec_append 4 ptr[6]", int, vec.ptr[6], 9);
	e_test_assert_eq ("e_vec_append 4 len", usize, vec.len, 7);
	e_test_assert_eq ("e_vec_append 4 cap", usize, vec.cap, 8);

	len = e_vec_int_pop_slice (&vec, &popped, 5);
	e_test_assert_eq ("e_vec_pop_slice 1 item", int, *popped, 3);
	e_test_assert_eq ("e_vec_pop_slice 1 len", usize, vec.len, 2);
	e_test_assert_eq ("e_vec_pop_slice 1 cap", usize, vec.cap, 8);
	e_test_assert_eq ("e_vec_pop_slice 1 returned len", usize, len, 5);

	e_vec_int_append (&vec, 10);
	e_test_assert_eq ("e_vec_append 5 ptr[2]", int, vec.ptr[2], 10);
	e_test_assert_eq ("e_vec_append 5 len", usize, vec.len, 3);
	e_test_assert_eq ("e_vec_append 5 cap", usize, vec.cap, 8);

	len = e_vec_int_pop_slice (&vec, &popped, 5);
	e_test_assert_eq ("e_vec_pop_slice 2 item", int, *popped, 1);
	e_test_assert_eq ("e_vec_pop_slice 2 len", usize, vec.len, 0);
	e_test_assert_eq ("e_vec_pop_slice 2 cap", usize, vec.cap, 8);
	e_test_assert_eq ("e_vec_pop_slice 2 returned len", usize, len, 3);

	e_vec_int_deinit (&vec);
}
