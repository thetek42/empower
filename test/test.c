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
	test_str ();
	test_vec ();

	e_test_finish ();
}

static void
test_arena (void)
{
	const char zero[2] = {0};
	E_Arena arena;
	void *ptr;

	arena = e_arena_init (16);

	ptr = e_arena_alloc (&arena, u32, 1);
	e_test_assert_ptr_eq ("e_arena_alloc ptr", ptr, arena.ptr);
	e_test_assert_eq ("e_arena_alloc align", uintptr_t, (uintptr_t) ptr % 4, 0);
	e_test_assert_eq ("e_arena_alloc len", usize, arena.len, 4);

	ptr = e_arena_alloc_zero (&arena, u16, 1);
	e_test_assert_ptr_eq ("e_arena_alloc_zero ptr", ptr, arena.ptr + 4);
	e_test_assert_eq ("e_arena_alloc_zero align", uintptr_t, (uintptr_t) ptr % 2, 0);
	e_test_assert_eq ("e_arena_alloc_zero len", usize, arena.len, 6);
	e_test_assert_eq ("e_arena_alloc_zero mem", int, memcmp (ptr, zero, 2), 0);

	ptr = e_arena_alloc_aligned (&arena, 8, 8);
	e_test_assert_ptr_eq ("e_arena_alloc_aligned ptr", ptr, arena.ptr + 8);
	e_test_assert_eq ("e_arena_alloc_aligned align", uintptr_t, (uintptr_t) ptr % 8, 0);
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
	e_test_assert_eq ("e_cstr_count_char_matching", usize, e_cstr_count_char_matching (s1, islower), 8);
	e_test_assert_eq ("e_cstr_count_str", usize, e_cstr_count_str (s4, "en"), 3);
	e_test_assert_eq ("e_cstr_count_str too long", usize, e_cstr_count_str (s3, "123456"), 0);
	e_test_assert ("e_cstr_is_blank false", bool, !e_cstr_is_blank (s1));
	e_test_assert ("e_cstr_is_blank true", bool, e_cstr_is_blank (s2));
	e_test_assert ("e_cstr_is_ascii false", bool, !e_cstr_is_ascii (s4));
	e_test_assert ("e_cstr_is_ascii true", bool, e_cstr_is_ascii (s1));
	e_test_assert_ptr_eq ("e_cstr_find_char existant", e_cstr_find_char (s1, 'o'), s1 + 4);
	e_test_assert_ptr_eq ("e_cstr_find_char nonexistant", e_cstr_find_char (s1, 'u'), nullptr);
	e_test_assert_ptr_eq ("e_cstr_find_char_matching existant", e_cstr_find_char_matching (s5, isalpha), s5 + 2);
	e_test_assert_ptr_eq ("e_cstr_find_char_matching nonexistant", e_cstr_find_char_matching (s5, isdigit), nullptr);
	e_test_assert_ptr_eq ("e_cstr_find_str existant", e_cstr_find_str (s4, "en"), s4 + 3);
	e_test_assert_ptr_eq ("e_cstr_find_str nonexistant", e_cstr_find_str (s5, "es"), nullptr);
	e_test_assert ("e_cstr_eq null null", bool, e_cstr_eq (nullptr, nullptr));
	e_test_assert ("e_cstr_eq ptr null", bool, !e_cstr_eq (s5, nullptr));
	e_test_assert ("e_cstr_eq ptr eq ptr", bool, e_cstr_eq (s5, s5));
	e_test_assert ("e_cstr_eq eq", bool, e_cstr_eq (s5, s7));
	e_test_assert ("e_cstr_eq not eq", bool, !e_cstr_eq (s5, s4));
	e_test_assert ("e_cstr_matches_predicate false", bool, !e_cstr_matches_predicate (s3, islower));
	e_test_assert ("e_cstr_matches_predicate true", bool, e_cstr_matches_predicate (s3, isdigit));
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
	E_Fs_File file;
	char *buf, buf2[16];
	usize len;

	e_test_assert_ok ("e_fs_file_open write", e_fs_file_open (&file, "test/test.txt", E_FS_OPEN_MODE_WRITE_TRUNC));
	e_test_assert_ok ("e_fs_file_write", e_fs_file_write (&file, "Hello, World!\n", strlen ("Hello, World!\n")));
	e_test_assert_ok ("e_fs_file_close write", e_fs_file_close (&file));

	e_test_assert_ok ("e_fs_file_open read", e_fs_file_open (&file, "test/test.txt", E_FS_OPEN_MODE_READ_ONLY));
	e_test_assert_ok ("e_fs_file_get_size", e_fs_file_get_size (&file, &len));
	e_test_assert_eq ("e_fs_file_get_size len", usize, len, strlen ("Hello, World!\n"));
	e_test_assert_ok ("e_fs_file_read 1", e_fs_file_read (&file, buf2, 4, &len));
	e_test_assert_eq ("e_fs_file_read 1 len", usize, len, strlen ("Hel"));
	e_test_assert_str_eq ("e_fs_file_read 1 out", buf2, "Hel");
	e_test_assert_ok ("e_fs_file_read 2", e_fs_file_read (&file, buf2, 3, &len));
	e_test_assert_eq ("e_fs_file_read 2 len", usize, len, strlen ("lo"));
	e_test_assert_str_eq ("e_fs_file_read 2 out", buf2, "lo");
	e_test_assert_ok ("e_fs_file_read 3", e_fs_file_read (&file, buf2, 16, &len));
	e_test_assert_eq ("e_fs_file_read 3 len", usize, len, strlen (", World!\n"));
	e_test_assert_str_eq ("e_fs_file_read 3 out", buf2, ", World!\n");
	e_test_assert_ok ("e_fs_file_read_all", e_fs_file_read_all (&file, &buf, &len));
	e_test_assert_str_eq ("e_fs_file_read_all out", buf, "Hello, World!\n");
	e_test_assert_eq ("e_fs_file_read_all len", usize, len, strlen ("Hello, World!\n"));
	e_test_assert_ok ("e_fs_file_close read", e_fs_file_close (&file));

	e_test_assert ("e_fs_path_exists true", bool, e_fs_path_exists ("test/test.txt"));
	e_test_assert ("e_fs_path_exists false", bool, !e_fs_path_exists ("eierschale.docx"));
	e_test_assert ("e_fs_is_file true", bool, e_fs_is_file ("test/test.txt"));
	e_test_assert ("e_fs_is_file false", bool, !e_fs_is_file ("test"));
	e_test_assert ("e_fs_is_dir true", bool, e_fs_is_dir ("test"));
	e_test_assert ("e_fs_is_dir false", bool, !e_fs_is_dir ("test/test.txt"));

	e_free (buf);
}

static void
test_ini (void)
{
	E_Ini ini;

	e_test_assert_ok ("e_ini_parse_file", e_ini_parse_file (&ini, "test/test.ini"));
	e_test_assert_str_eq ("e_ini_get_entry no_section", e_ini_get_entry (&ini, nullptr, "no_section")->value, "123");
	e_test_assert_str_eq ("e_ini_get_entry section", e_ini_get_entry (&ini, "bar", "rab")->value, "zab");
	e_test_assert_str_eq ("e_ini_get_entry empty", e_ini_get_entry (&ini, "foo", "qux")->value, "");
	e_test_assert_ptr_eq ("e_ini_get_entry invalid section", e_ini_get_entry (&ini, "kjfadsf", "foo"), nullptr);
	e_test_assert_ptr_eq ("e_ini_get_entry invalid key", e_ini_get_entry (&ini, "foo", "kjfadsf"), nullptr);
	e_ini_deinit (&ini);
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
	e_test_assert_ptr_eq ("e_vec_init ptr", vec.ptr, nullptr);
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
