#include "empower.h"

static void test_arena (void);
static void test_cstr (void);
static void test_enc (void);
static void test_fs (void);
static void test_str (void);

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
	test_str ();

	e_test_finish ();
}

static void
test_arena (void)
{
	const char zero[2] = {0};
	uintptr_t unalignment;
	E_Arena arena;
	void *ptr;

	arena = e_arena_init ();

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
