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
	uintptr_t unalignment;
	E_Arena arena;
	void *ptr;

	arena = e_arena_init (16);

	ptr = e_arena_alloc (&arena, u32, 1);
	unalignment = (uintptr_t) ptr % 4;
	e_test_assert_eq ("e_arena_alloc ptr", ptr, arena.ptr);
	e_test_assert_eq ("e_arena_alloc align", unalignment, 0);
	e_test_assert_eq ("e_arena_alloc len", arena.len, 4);

	ptr = e_arena_alloc_zero (&arena, u16, 1);
	unalignment = (uintptr_t) ptr % 2;
	e_test_assert_eq ("e_arena_alloc_zero ptr", ptr, arena.ptr + 4);
	e_test_assert_eq ("e_arena_alloc_zero align", unalignment, 0);
	e_test_assert_eq ("e_arena_alloc_zero len", arena.len, 6);
	e_test_assert_eq ("e_arena_alloc_zero mem", memcmp (ptr, zero, 2), 0);

	ptr = e_arena_alloc_aligned (&arena, 8, 8);
	unalignment = (uintptr_t) ptr % 8;
	e_test_assert_eq ("e_arena_alloc_aligned ptr", ptr, arena.ptr + 8);
	e_test_assert_eq ("e_arena_alloc_aligned align", unalignment, 0);
	e_test_assert_eq ("e_arena_alloc_aligned len", arena.len, 16);

	e_arena_deinit (&arena);
}

static void
test_cstr (void)
{
	char s1[] = "Hello, World!";
	char s2[] = " \n\t \f \v    \r ";
	char s3[] = "1337";
	char s4[] = "Dosenöffner";
	char s5[] = "  foo   ";
	char s6[] = "";
	usize len;

	e_test_assert_eq ("e_cstr_count_char", e_cstr_count_char (s1, 'l'), 3);
	e_test_assert_eq ("e_cstr_count_char nul", e_cstr_count_char (s1, '\0'), 0);
	e_test_assert_eq ("e_cstr_count_func", e_cstr_count_func (s1, islower), 8);
	e_test_assert ("e_cstr_is_blank false", !e_cstr_is_blank (s1));
	e_test_assert ("e_cstr_is_blank true", e_cstr_is_blank (s2));
	e_test_assert ("e_cstr_is_ascii false", !e_cstr_is_ascii (s4));
	e_test_assert ("e_cstr_is_ascii true", e_cstr_is_ascii (s1));
	e_test_assert ("e_cstr_matches_predicate false", !e_cstr_matches_predicate (s3, islower));
	e_test_assert ("e_cstr_matches_predicate true", e_cstr_matches_predicate (s3, isdigit));
	e_test_assert_str_eq ("e_cstr_to_ascii_lower", e_cstr_to_ascii_lower (s1), "hello, world!");
	e_test_assert_str_eq ("e_cstr_to_ascii_upper", e_cstr_to_ascii_upper (s1), "HELLO, WORLD!");
	e_test_assert_str_eq ("e_cstr_trim_start", e_cstr_trim_start (s5), s5 + 2);
	e_test_assert_str_eq ("e_cstr_trim_start empty", e_cstr_trim_start (s6), s6);
	e_test_assert_eq ("e_cstr_trim_end", e_cstr_trim_end (s5), strlen (s5) - 3);
	e_test_assert_eq ("e_cstr_trim_end empty", e_cstr_trim_end (s6), 0);
	e_test_assert_str_eq ("e_cstr_trim", e_cstr_trim (s5, &len), s5 + 2);
	e_test_assert_eq ("e_cstr_trim len", len, 3);
	e_test_assert_str_eq ("e_cstr_trim empty", e_cstr_trim (s6, &len), s6);
	e_test_assert_eq ("e_cstr_trim empty len", len, 0);
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
	char *buf;
	usize len;

	e_test_assert_ok ("e_fs_file_open write", e_fs_file_open (&file, "test/test.txt", E_FS_OPEN_MODE_WRITE_TRUNC));
	e_test_assert_ok ("e_fs_file_write", e_fs_file_write (&file, "Hello, World!\n", strlen ("Hello, World!\n")));
	e_test_assert_ok ("e_fs_file_close write", e_fs_file_close (&file));

	e_test_assert_ok ("e_fs_file_open read", e_fs_file_open (&file, "test/test.txt", E_FS_OPEN_MODE_READ_ONLY));
	e_test_assert_ok ("e_fs_file_get_size", e_fs_file_get_size (&file, &len));
	e_test_assert_eq ("e_fs_file_get_size len", len, strlen ("Hello, World!\n"));
	e_test_assert_ok ("e_fs_file_read_all", e_fs_file_read_all (&file, &buf, &len));
	e_test_assert_ok ("e_fs_file_close read", e_fs_file_close (&file));
	e_test_assert_str_eq ("e_fs_file_read_all out", buf, "Hello, World!\n");
	e_test_assert_eq ("e_fs_file_read_all len", len, strlen ("Hello, World!\n"));

	e_test_assert ("e_fs_path_exists true", e_fs_path_exists ("test/test.txt"));
	e_test_assert ("e_fs_path_exists false", !e_fs_path_exists ("eierschale.docx"));
	e_test_assert ("e_fs_is_file true", e_fs_is_file ("test/test.txt"));
	e_test_assert ("e_fs_is_file false", !e_fs_is_file ("test"));
	e_test_assert ("e_fs_is_dir true", e_fs_is_dir ("test"));
	e_test_assert ("e_fs_is_dir false", !e_fs_is_dir ("test/test.txt"));

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
	e_test_assert_eq ("e_ini_get_entry invalid section", (void *) e_ini_get_entry (&ini, "kjfadsf", "foo"), nullptr);
	e_test_assert_eq ("e_ini_get_entry invalid key", (void *) e_ini_get_entry (&ini, "foo", "kjfadsf"), nullptr);
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
	e_test_assert_eq ("e_str_append_cstr len", str.len, strlen ("Hello, World"));
	e_test_assert_eq ("e_str_append_cstr len_ret", len_ret, strlen ("Hello, World"));

	e_str_append_char (&str, '!');
	e_test_assert_str_eq ("e_str_append_char", str.ptr, "Hello, World!");
	e_test_assert_eq ("e_str_append_char len", str.len, strlen ("Hello, World!"));

	e_str_append_slice (&str, " 12345", 4);
	e_test_assert_str_eq ("e_str_append_slice", str.ptr, "Hello, World! 123");
	e_test_assert_eq ("e_str_append_char len", str.len, strlen ("Hello, World! 123"));

	len_ret = e_str_append_fmt (&str, 32, " dec:%d,hex:%x", 42, 42);
	e_test_assert_str_eq ("e_str_append_fmt", str.ptr, "Hello, World! 123 dec:42,hex:2a");
	e_test_assert_eq ("e_str_append_fmt len", str.len, strlen ("Hello, World! 123 dec:42,hex:2a"));
	e_test_assert_eq ("e_str_append_fmt len_ret", len_ret, strlen (" dec:42,hex:2a"));

	len_ret = e_str_append_fmt (&str, 4, " %d", 12345);
	e_test_assert_str_eq ("e_str_append_fmt capped", str.ptr, "Hello, World! 123 dec:42,hex:2a 123");
	e_test_assert_eq ("e_str_append_fmt capped len", str.len, strlen ("Hello, World! 123 dec:42,hex:2a 123"));
	e_test_assert_eq ("e_str_append_fmt capped len_ret", len_ret, 6);

	len_ret = e_str_append_fmt_all (&str, " %d", 12345);
	e_test_assert_str_eq ("e_str_append_fmt_all", str.ptr, "Hello, World! 123 dec:42,hex:2a 123 12345");
	e_test_assert_eq ("e_str_append_fmt_all len", str.len, strlen ("Hello, World! 123 dec:42,hex:2a 123 12345"));
	e_test_assert_eq ("e_str_append_fmt_all len_ret", len_ret, 6);

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
	e_test_assert_eq ("e_vec_init ptr", (void *) vec.ptr, nullptr);
	e_test_assert_eq ("e_vec_init len", vec.len, 0);
	e_test_assert_eq ("e_vec_init cap", vec.cap, 0);

	e_vec_int_append (&vec, 1);
	e_test_assert_eq ("e_vec_append 1 ptr[0]", vec.ptr[0], 1);
	e_test_assert_eq ("e_vec_append 1 len", vec.len, 1);
	e_test_assert_eq ("e_vec_append 1 cap", vec.cap, 1);

	e_vec_int_append (&vec, 2);
	e_test_assert_eq ("e_vec_append 2 ptr[1]", vec.ptr[1], 2);
	e_test_assert_eq ("e_vec_append 2 len", vec.len, 2);
	e_test_assert_eq ("e_vec_append 2 cap", vec.cap, 2);

	e_vec_int_append (&vec, 3);
	e_test_assert_eq ("e_vec_append 3 ptr[2]", vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_append 3 len", vec.len, 3);
	e_test_assert_eq ("e_vec_append 3 cap", vec.cap, 4);

	e_vec_int_append_slice (&vec, slice, 5);
	e_test_assert_eq ("e_vec_append_slice ptr[3]", vec.ptr[3], 4);
	e_test_assert_eq ("e_vec_append_slice ptr[7]", vec.ptr[7], 8);
	e_test_assert_eq ("e_vec_append_slice len", vec.len, 8);
	e_test_assert_eq ("e_vec_append_slice cap", vec.cap, 8);

	e_vec_int_pop (&vec, &popped);
	e_test_assert_eq ("e_vec_pop 1 item", *popped, 8);
	e_test_assert_eq ("e_vec_pop 1 len", vec.len, 7);
	e_test_assert_eq ("e_vec_pop 1 cap", vec.cap, 8);

	e_vec_int_pop (&vec, &popped);
	e_test_assert_eq ("e_vec_pop 2 item", *popped, 7);
	e_test_assert_eq ("e_vec_pop 2 len", vec.len, 6);
	e_test_assert_eq ("e_vec_pop 2 cap", vec.cap, 8);

	e_vec_int_append (&vec, 9);
	e_test_assert_eq ("e_vec_append 4 ptr[6]", vec.ptr[6], 9);
	e_test_assert_eq ("e_vec_append 4 len", vec.len, 7);
	e_test_assert_eq ("e_vec_append 4 cap", vec.cap, 8);

	len = e_vec_int_pop_slice (&vec, &popped, 5);
	e_test_assert_eq ("e_vec_pop_slice 1 item", *popped, 3);
	e_test_assert_eq ("e_vec_pop_slice 1 len", vec.len, 2);
	e_test_assert_eq ("e_vec_pop_slice 1 cap", vec.cap, 8);
	e_test_assert_eq ("e_vec_pop_slice 1 returned len", len, 5);

	e_vec_int_append (&vec, 10);
	e_test_assert_eq ("e_vec_append 5 ptr[2]", vec.ptr[2], 10);
	e_test_assert_eq ("e_vec_append 5 len", vec.len, 3);
	e_test_assert_eq ("e_vec_append 5 cap", vec.cap, 8);

	len = e_vec_int_pop_slice (&vec, &popped, 5);
	e_test_assert_eq ("e_vec_pop_slice 2 item", *popped, 1);
	e_test_assert_eq ("e_vec_pop_slice 2 len", vec.len, 0);
	e_test_assert_eq ("e_vec_pop_slice 2 cap", vec.cap, 8);
	e_test_assert_eq ("e_vec_pop_slice 2 returned len", len, 3);

	e_vec_int_deinit (&vec);
}
