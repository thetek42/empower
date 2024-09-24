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
 *  - e_alloc
 *  - e_result
 *  - e_log (transitive)
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
[[nodiscard]]
E_Result e_fs_file_open (E_Fs_File *file_out, const char *path, E_Fs_Open_Mode mode);

/**
 * Close the file handle \file. This function is essentially a wrapper around
 * `fclose`. Returns E_OK or the error that occured during `fclose`.
 */
[[nodiscard]]
E_Result e_fs_file_close (E_Fs_File *file);

/**
 * Get the size of the file \file and store it in \size_out. Returns E_OK or an
 * error that occured during `fseek` or `ftell`.
 */
[[nodiscard]]
E_Result e_fs_file_get_size (E_Fs_File *file, usize *size_out);

/**
 * Reads all content the file handle \file into a buffer pointed to by \out. A
 * terminating nul byte will be written. The length (excluding the terminating
 * nul byte) will be stored in \len_out. If the length is not required, \len_out
 * can be `nullptr`. The required memory is allocted using `e_alloc` and must be
 * freed by the user using `e_free`. If an error occurs, the memory is freed
 * automatically, \out is set to `nullptr`, \len_out is set to 0 and an error is
 * returned.
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
[[nodiscard]]
E_Result e_fs_file_read_all (E_Fs_File *file, char **out, usize *len_out);

/**
 * Write \len bytes of data pointed to by \data to a file \file. Returns E_OK on
 * success, and an error on failure.
 */
[[nodiscard]]
E_Result e_fs_file_write (E_Fs_File *file, const char *data, usize len);

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
