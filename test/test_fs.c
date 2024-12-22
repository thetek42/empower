#define E_FS_IMPL
#include <e_fs.h>

#include <e_test.h>
#include <e_types.h>
#include <stdlib.h>

void
test_fs (void)
{
	E_File file;
	char *buf, buf2[15];
	usize len;

	e_test_assert_ok ("e_fs_file_open write", e_fs_file_open (&file, "test/util/test.txt", E_FS_OPEN_MODE_WRITE_TRUNC));
	e_test_assert_ok ("e_fs_file_write", e_fs_file_write (&file, "Hello, World!\t", strlen ("Hello, World!\t")));
	e_test_assert_ok ("e_fs_file_write_fmt", e_fs_file_write_fmt (&file, &len, "%d", 42));
	e_test_assert_eq ("e_fs_file_write_fmt len", usize, len, 2);
	e_fs_file_close (&file);

	e_test_assert_ok ("e_fs_file_open read", e_fs_file_open (&file, "test/util/test.txt", E_FS_OPEN_MODE_READ_ONLY));

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
	free (buf);

	e_test_assert_ok ("e_fs_file_read_all", e_fs_file_read_all (&file, &buf, &len));
	e_test_assert_str_eq ("e_fs_file_read_all out", buf, "Hello, World!\t42");
	e_test_assert_eq ("e_fs_file_read_all len", usize, len, strlen ("Hello, World!\t42"));
	free (buf);
	e_fs_file_close (&file);

	e_test_assert ("e_fs_path_exists true", e_fs_path_exists ("test/util/test.txt"));
	e_test_assert ("e_fs_path_exists false", !e_fs_path_exists ("eierschale.docx"));
	e_test_assert ("e_fs_is_file true", e_fs_is_file ("test/util/test.txt"));
	e_test_assert ("e_fs_is_file false", !e_fs_is_file ("test"));
	e_test_assert ("e_fs_is_dir true", e_fs_is_dir ("test"));
	e_test_assert ("e_fs_is_dir false", !e_fs_is_dir ("test/util/test.txt"));
}
