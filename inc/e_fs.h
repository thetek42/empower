#ifndef _EMPOWER_FS_H_
#define _EMPOWER_FS_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_FS

#if !E_CONFIG_MODULE_ALLOC || !E_CONFIG_MODULE_RESULT
# error "module e_fs depends on: e_alloc, e_result"
#endif /* !E_CONFIG_MODULE_ALLOC || !E_CONFIG_MODULE_RESULT */

/*! e_fs **********************************************************************
 * 
 * This module provides wrappers for file system APIs.
 *
 * Module dependencies:
 *  - e_result
 *  - e_alloc (optional)
 *  - e_log (optional; transitive)
 *
 ******************************************************************************/

/**
 * Specifies which mode to use for opening files.
 */
/* TODO: this should be done with flags instead */
typedef enum {
	E_FS_OPEN_MODE_READ_ONLY, /* r */
	E_FS_OPEN_MODE_WRITE_TRUNC, /* w */
	E_FS_OPEN_MODE_WRITE_APPEND, /* a */
	E_FS_OPEN_MODE_READ_WRITE, /* r+ */
	E_FS_OPEN_MODE_READ_WRITE_TRUNC, /* w+ */
	E_FS_OPEN_MODE_READ_WRITE_APPEND, /* a+ */
} E_Fs_Open_Mode;

/**
 * Specifies the permissions of a file or directory.
 */
typedef enum {
	/* individual options */
	E_FS_PERM_USER_READ = S_IRUSR,
	E_FS_PERM_USER_WRITE = S_IWUSR,
	E_FS_PERM_USER_EXEC = S_IXUSR,
	E_FS_PERM_GROUP_READ = S_IRGRP,
	E_FS_PERM_GROUP_WRITE = S_IWGRP,
	E_FS_PERM_GROUP_EXEC = S_IXGRP,
	E_FS_PERM_OTHER_READ = S_IROTH,
	E_FS_PERM_OTHER_WRITE = S_IWOTH,
	E_FS_PERM_OTHER_EXEC = S_IXOTH,
	/* combined options */
	E_FS_PERM_USER_RWX = S_IRWXU,
	E_FS_PERM_GROUP_RWX = S_IRWXG,
	E_FS_PERM_OTHER_RWX = S_IRWXO,
} E_Fs_Permissions;

/**
 * A wrapper for a `FILE` handle.
 */
typedef struct {
	FILE *handle;
} E_File;

/**
 * Reads the entire content of the file with path \path into a buffer pointed to
 * by \out. The file will be opened and closed automatically. The behaviour and
 * arguments of this function are essentially the same as `e_fs_file_read_all`.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_read_file (const char *path, char **out, usize *len_out);

/**
 * Open a file handle for the file located at \path using the open mode \mode.
 * This function is essentially a wrapper around `fopen`. The file handle will
 * be put into \file. The function returns an `E_Result` with E_OK in case of
 * success and an error in case `fopen` failed.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_open (E_File *file_out, const char *path, E_Fs_Open_Mode mode);

/**
 * Close the file handle \file. This function is essentially a wrapper around
 * `fclose`. Returns E_OK or the error that occured during `fclose`.
 */
void e_fs_file_close (E_File *file);

/**
 * Get the total size of the file \file and store it in \size_out. Returns E_OK or an
 * error that occured during `fseek` or `ftell`.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_get_size (E_File *file, usize *size_out);

/**
 * Get the remaining size (i.e. from the current file position indicator to the
 * end of the file) of the file \file and store it in \size_out. Returns E_OK or
 * an error that occured during `fseek` or `ftell`.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_get_remaining_size (E_File *file, usize *size_out);

/**
 * Get the file position indicator of the file \file and stores it in \pos. If
 * an error occurs, \pos is set to 0 and the error is returned.
 */
E_Result
e_fs_file_get_pos (E_File *file, usize *pos);

/**
 * Set the file position indicator of the file \file to \pos bytes after the
 * beginning of the file. Returns E_OK or an error that occured during `fseek`.
 */
E_Result
e_fs_file_set_pos (E_File *file, usize pos);

/**
 * Rewind the file \file, i.e. set its position indicator to \pos bytes before
 * the end of the file. the file. Only returns an error if \file is `nullptr`.
 * Note that this behaves differently than a regular `fseek` with `SEEK_END` in
 * that `fseek` expects \pos to be negative, while this function expects it to
 * be positive.
 */
E_Result
e_fs_file_set_pos_end (E_File *file, usize pos);

/**
 * Rewind the file \file, i.e. set its position indicator to the beginning of
 * the file. Only returns an error if \file is `nullptr`.
 */
E_Result
e_fs_file_rewind (E_File *file);

/**
 * Reads up to \max_len number of bytes of the file handle \file into the buffer
 * pointed to by \out, and advances the position in the file such that
 * subsequent read calls will read the next parts of the file. A terminating nul
 * byte will be written. The length (excluding the terminating nul byte) will be
 * stored in \len_out. If the length is not required, \len_out can be `nullptr`.
 * \file or \out may not be `nullptr`, otherwise the function will return an
 * error. If an error occurs, \len_out will be set to 0, and \out will be filled
 * with only the terminating nul byte. \out must be able to store at least
 * \max_len + 1 bytes. \max_len includes the nul terminator. If \max_len is 0,
 * the terminating nul byte is not written to \out.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_read (E_File *file, char *out, usize max_len, usize *len_out);

#if E_CONFIG_MODULE_ALLOC

/**
 * Reads the entire content of the file handle \file into a buffer pointed to by
 * \out. A terminating nul byte will be written. The length (excluding the
 * terminating nul byte) will be stored in \len_out. If the length is not
 * required, \len_out can be `nullptr`. The required memory is allocted using
 * `e_alloc` and must be freed by the user using `e_free`. If an error occurs,
 * the memory is freed automatically, \out is set to `nullptr`, \len_out is set
 * to 0 and an error is returned. The position of the file will be advanced to
 * the end.
 *
 * Example (without error handling, indicated by `(void)`):
 * | E_Fs_File file;
 * | char *buf;
 * | usize len;
 * | (void) e_fs_file_open (&file, "data.txt", E_FS_OPEN_MODE_READ_ONLY);
 * | (void) e_fs_file_read_all (&file, &buf, &len);
 * | e_fs_file_close (&file);
 * | printf ("len: %zu, contents:\n%s\n", len, buf);
 * | e_free (buf);
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_read_all (E_File *file, char **out, usize *len_out);

/**
 * Reads the remaining content (i.e. from the current file position indicator
 * to the end of the file) of the file handle \file into an allocated buffer.
 * The behaviour of this function is the same as `e_fs_file_read_all`, except
 * that it only reads the remaining content and not the entire file.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_read_remaining (E_File *file, char **out, usize *len_out);

#endif /* E_CONFIG_MODULE_ALLOC */

/**
 * Write \len bytes of data pointed to by \data to a file \file. Returns E_OK on
 * success, and an error on failure.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_write (E_File *file, const char *data, usize len);

/**
 * Write a formatted string to the file handle \file. This uses the same format
 * as `fprintf` with \fmt being the format string. If \written is not `nullptr`,
 * the number of bytes written will be stored in \written. If an error occurs,
 * \written is set to 0.
 */
E_ATTR_FORMAT (printf, 3, 4)
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_write_fmt (E_File *file, usize *written, const char *fmt, ...);

/**
 * Check if a path exists.
 */
bool e_fs_path_exists (const char *path);

/**
 * Check if a path points to a file.
 */
bool e_fs_is_file (const char *path);

/**
 * Check if a path points to a file.
 */
bool e_fs_is_dir (const char *path);

/**
 * Check if a path points to a file.
 */
bool e_fs_is_link (const char *path);

/**
 * Create a directory at path \path with permissions \perm.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_dir_create (const char *path, E_Fs_Permissions perm);

#endif /* E_CONFIG_MODULE_FS */
#endif /* _EMPOWER_FS_H_ */
