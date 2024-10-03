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
 * A wrapper for a `FILE` handle.
 */
typedef struct {
	FILE *handle;
} E_Fs_File;

/**
 * Open a file handle for the file located at \path using the open mode \mode.
 * This function is essentially a wrapper around `fopen`. The file handle will
 * be put into \file. The function returns an `E_Result` with E_OK in case of
 * success and an error in case `fopen` failed.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_open (E_Fs_File *file_out, const char *path, E_Fs_Open_Mode mode);

/**
 * Close the file handle \file. This function is essentially a wrapper around
 * `fclose`. Returns E_OK or the error that occured during `fclose`.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_close (E_Fs_File *file);

/**
 * Get the size of the file \file and store it in \size_out. Returns E_OK or an
 * error that occured during `fseek` or `ftell`.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_get_size (E_Fs_File *file, usize *size_out);

/**
 * Rewind the file \file, i.e. set its position indicator to the beginning of
 * the file. Only returns an error if \file is `nullptr`.
 */
E_Result
e_fs_file_rewind (E_Fs_File *file);

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
E_Result e_fs_file_read (E_Fs_File *file, char *out, usize max_len, usize *len_out);

#if E_CONFIG_MODULE_ALLOC

/**
 * Reads the entire contents of the file handle \file into a buffer pointed to
 * by \out. A terminating nul byte will be written. The length (excluding the
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
 * | (void) e_fs_file_close (&file);
 * | printf ("len: %zu, contents:\n%s\n", len, buf);
 * | e_free (buf);
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_read_all (E_Fs_File *file, char **out, usize *len_out);

#endif /* E_CONFIG_MODULE_ALLOC */

/**
 * Write \len bytes of data pointed to by \data to a file \file. Returns E_OK on
 * success, and an error on failure.
 */
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_write (E_Fs_File *file, const char *data, usize len);

/**
 * Write a formatted string to the file handle \file. This uses the same format
 * as `fprintf` with \fmt being the format string. If \written is not `nullptr`,
 * the number of bytes written will be stored in \written. If an error occurs,
 * \written is set to 0.
 */
E_ATTR_FORMAT (printf, 3, 4)
E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_write_fmt (E_Fs_File *file, usize *written, const char *fmt, ...);

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

#endif /* E_CONFIG_MODULE_FS */
#endif /* _EMPOWER_FS_H_ */
