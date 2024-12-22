#define E_FS_IMPL
#include <e_fs.h>

#include <e_test.h>
#include <e_types.h>
#include <stdio.h>
#include <stdlib.h>

void
test_fs (void)
{
	char *buf1, *buf2, buf3[15];
	E_Fs_Permissions perm;
	E_Result res;
	E_File file;
	usize len;

	if (e_fs_is_dir ("tmp")) {
		res = e_fs_remove ("tmp");
		if (res != E_OK) {
			// TODO: delete from code
			fprintf (stderr, "please remove the tmp dir");
			exit (EXIT_FAILURE);
		}
	}

	perm = E_FS_PERM_USER_RWX | E_FS_PERM_GROUP_READ | E_FS_PERM_GROUP_EXEC | E_FS_PERM_OTHER_READ | E_FS_PERM_OTHER_EXEC;
	e_test_assert_ok ("e_fs_dir_create", e_fs_dir_create ("tmp", perm));
	e_test_assert ("e_fs_dir_create success", e_fs_is_dir ("tmp"));

	e_test_assert_ok ("e_fs_file_open write", e_fs_file_open (&file, "tmp/test.txt", E_FS_OPEN_MODE_WRITE_TRUNC));
	e_test_assert_ok ("e_fs_file_write", e_fs_file_write (&file, "Hello, World!\t", strlen ("Hello, World!\t")));
	e_test_assert_ok ("e_fs_file_write_fmt", e_fs_file_write_fmt (&file, &len, "%d", 42));
	e_test_assert_eq ("e_fs_file_write_fmt len", usize, len, 2);
	e_fs_file_close (&file);

	e_test_assert_ok ("e_fs_file_open read", e_fs_file_open (&file, "tmp/test.txt", E_FS_OPEN_MODE_READ_ONLY));

	e_test_assert_ok ("e_fs_file_get_size", e_fs_file_get_size (&file, &len));
	e_test_assert_eq ("e_fs_file_get_size len", usize, len, strlen ("Hello, World!\t42"));
	e_test_assert_ok ("e_fs_file_read 1", e_fs_file_read (&file, buf3, 15, &len));
	e_test_assert_eq ("e_fs_file_read 1 len", usize, len, strlen ("Hello, World!\t"));
	e_test_assert_str_eq ("e_fs_file_read 1 out", buf3, "Hello, World!\t");
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
	e_test_assert_ok ("e_fs_file_read 2", e_fs_file_read (&file, buf3, 15, &len));
	e_test_assert_eq ("e_fs_file_read 2 len", usize, len, strlen ("rld!\t42"));
	e_test_assert_str_eq ("e_fs_file_read 2 out", buf3, "rld!\t42");
	e_test_assert_ok ("e_fs_file_rewind", e_fs_file_rewind (&file));
	e_test_assert_ok ("e_fs_file_read 3", e_fs_file_read (&file, buf3, 8, &len));
	e_test_assert_eq ("e_fs_file_read 3 len", usize, len, strlen ("Hello, "));

	e_test_assert_str_eq ("e_fs_file_read 3 out", buf3, "Hello, ");
	e_test_assert_ok ("e_fs_file_read_remaining", e_fs_file_read_remaining (&file, &buf1, &len));
	e_test_assert_str_eq ("e_fs_file_read_remaining out", buf1, "World!\t42");
	e_test_assert_eq ("e_fs_file_read_remaining len", usize, len, strlen ("World!\t42"));
	free (buf1);

	e_test_assert_ok ("e_fs_file_read_all", e_fs_file_read_all (&file, &buf1, &len));
	e_test_assert_str_eq ("e_fs_file_read_all out", buf1, "Hello, World!\t42");
	e_test_assert_eq ("e_fs_file_read_all len", usize, len, strlen ("Hello, World!\t42"));
	free (buf1);
	e_fs_file_close (&file);

	e_test_assert ("e_fs_path_exists true", e_fs_path_exists ("tmp/test.txt"));
	e_test_assert ("e_fs_path_exists false", !e_fs_path_exists ("eierschale.docx"));
	e_test_assert ("e_fs_is_file true", e_fs_is_file ("tmp/test.txt"));
	e_test_assert ("e_fs_is_file false", !e_fs_is_file ("test"));
	e_test_assert ("e_fs_is_dir true", e_fs_is_dir ("test"));
	e_test_assert ("e_fs_is_dir false", !e_fs_is_dir ("tmp/test.txt"));

	e_test_assert_ok ("e_fs_move", e_fs_move ("tmp/test.txt", "tmp/moved.txt"));
	e_test_assert ("e_fs_move success 1", e_fs_is_file ("tmp/moved.txt"));
	e_test_assert ("e_fs_move success 2", !e_fs_is_file ("tmp/test.txt"));
	e_test_assert_ok ("e_fs_copy", e_fs_copy ("tmp/moved.txt", "tmp/test.txt"));
	e_test_assert_ok ("e_fs_copy read 1", e_fs_read_file ("tmp/moved.txt", &buf1, nullptr));
	e_test_assert_ok ("e_fs_copy read 2", e_fs_read_file ("tmp/test.txt", &buf2, nullptr));
	e_test_assert_str_eq ("e_fs_copy eq", buf1, buf2);
	free (buf1);
	free (buf2);

	e_test_assert_err ("e_fs_remove dir nonempty", e_fs_remove ("tmp"));
	e_test_assert_ok ("e_fs_remove file 1", e_fs_remove ("tmp/test.txt"));
	e_test_assert_ok ("e_fs_remove file 2", e_fs_remove ("tmp/moved.txt"));
	e_test_assert_ok ("e_fs_remove dir empty", e_fs_remove ("tmp"));
	e_test_assert_err ("e_fs_remove nonexistant", e_fs_remove ("eierschale.txt"));
	e_test_assert ("e_fs_remove success", !e_fs_path_exists ("tmp"));
}
