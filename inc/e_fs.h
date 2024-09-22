#ifndef _EMPOWER_FS_H_
#define _EMPOWER_FS_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_FS

#if !E_CONFIG_MODULE_ALLOC || !E_CONFIG_MODULE_RESULT
# error "module e_alloc depends on: e_alloc"
#endif /* !E_CONFIG_MODULE_ALLOC */

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
} e_fs_open_mode_t;

typedef struct {
	FILE *handle;
} e_fs_file_t;

/**
 * Open a file handle. This function is essentially a wrapper around `fopen`.
 *
 * @param file: The file handle output
 * @param path: The path to the file
 * @param mode: The open mode (read, write, ...)
 * @return E_OK or error that occured during `fopen`.
 */
[[nodiscard]]
e_result_t e_fs_file_open (e_fs_file_t *file_out, const char *path, e_fs_open_mode_t mode);

/**
 * Close a file handle. This function is essentially a wrapper around `fclose`.
 *
 * @param file: The file to close
 * @return E_OK or error that occured during `fclose`.
 */
[[nodiscard]]
e_result_t e_fs_file_close (e_fs_file_t *file);

/**
 * Get the size of a file.
 *
 * @param file: The file to inspect
 * @param size_out: The pointer to where to size should be stored. If an error
 *                  occurs, the size will be set to 0.
 * @return E_OK or error that occured during `fseek` or `ftell`.
 */
[[nodiscard]]
e_result_t e_fs_file_get_size (e_fs_file_t *file, usize *size_out);

/**
 * Reads all content a file handle into a buffer. The required memory is
 * allocted using `e_alloc` and must be freed by the user using `e_free`. If an
 * error occurs, the memory is freed automatically.
 *
 * Example (without error handling, indicated by `(void)`):
 * | e_fs_file_t file;
 * | char *buf;
 * | usize len;
 * | (void) e_fs_file_open (&file, "data.txt", E_FS_OPEN_MODE_READ_ONLY);
 * | (void) e_fs_file_read_all (&file, &buf, &len);
 * | (void) e_fs_file_close (&file);
 * | printf ("len: %zu, contents:\n%s", len, buf);
 * | e_free (buf);
 *
 * @param file: The file to read from
 * @param out: A pointer to the pointer which will be set to the allocated
 *             memory location. After the read succeeds, this will contain the
 *             file contents and a terminating nul byte. In case of an error, it
 *             will be set to `nullptr`.
 * @param len_out: A pointer to the variable which will be set to the length of
 *                 the file contents (excluding terminating nul byte in the
 *                 output). In case of an error, it will be set to 0. `len_out`
 *                 can be set to `nullptr` if the length is not needed.
 * @return E_OK or any error that occured during `fseek` or `ftell`. If an error
 *         occurs during `fread`, it is reported as \E_ERR_FAIL.
 */
[[nodiscard]]
e_result_t e_fs_file_read_all (e_fs_file_t *file, char **out, usize *len_out);

/**
 * Write bytes to a file.
 *
 * @param file: The file to write to
 * @param data: A pointer to the data
 * @param len: The length of \data.
 * @return E_OK, E_ERR_INVALID_ARGUMENT, or in case the call to `fwrite` fails,
 *         E_ERR_FAIL.
 */
[[nodiscard]]
e_result_t e_fs_file_write (e_fs_file_t *file, const char *data, usize len);

/**
 * Check if a path exists.
 *
 * @param path: The path which should be checked
 */
bool e_fs_path_exists (const char *path);

/**
 * Check if a path points to a file.
 *
 * @param path: The path which should be checked
 */
bool e_fs_is_file (const char *path);

/**
 * Check if a path points to a file.
 *
 * @param path: The path which should be checked
 */
bool e_fs_is_dir (const char *path);

/**
 * Check if a path points to a file.
 *
 * @param path: The path which should be checked
 */
bool e_fs_is_link (const char *path);

#endif /* E_CONFIG_MODULE_FS */
#endif /* _EMPOWER_FS_H_ */
