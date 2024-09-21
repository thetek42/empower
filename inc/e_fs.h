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
E_ATTR_ACCESS_WRITE_ONLY (1)
E_ATTR_ACCESS_READ_ONLY (2)
E_ATTR_NUL_STRING_ARG (2)
E_ATTR_NONNULL_2 (1, 2)
e_result_t e_fs_file_open (e_fs_file_t *file_out, const char *path, e_fs_open_mode_t mode);

/**
 * Close a file handle. This function is essentially a wrapper around `fclose`.
 *
 * @param file: The file to close
 * @return E_OK or error that occured during `fclose`.
 */
E_ATTR_NONNULL (1)
e_result_t e_fs_file_close (e_fs_file_t *file);

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
E_ATTR_ACCESS_WRITE_ONLY (2)
E_ATTR_ACCESS_WRITE_ONLY (3)
E_ATTR_ACCESS_READ_ONLY (1)
E_ATTR_NONNULL_2 (1, 2)
e_result_t e_fs_file_read_all (e_fs_file_t *file, char **out, usize *len_out);

#endif /* E_CONFIG_MODULE_FS */
#endif /* _EMPOWER_FS_H_ */
