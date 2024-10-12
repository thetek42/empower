#include "e_fs.h"

#if E_CONFIG_MODULE_FS

#include <limits.h>
#include <stdarg.h>
#include <unistd.h>

static const char *const mode_str_table[] = {
	"r", /* E_FS_OPEN_MODE_READ_ONLY */
	"w", /* E_FS_OPEN_MODE_WRITE_TRUNC */
	"a", /* E_FS_OPEN_MODE_WRITE_APPEND */
	"r+", /* E_FS_OPEN_MODE_READ_WRITE */
	"w+", /* E_FS_OPEN_MODE_READ_WRITE_TRUNC */
	"a+", /* E_FS_OPEN_MODE_READ_WRITE_APPEND */
};

/**
 * Open a file handle for the file located at \path using the open mode \mode.
 * This function is essentially a wrapper around `fopen`. The file handle will
 * be put into \file. The function returns an `E_Result` with E_OK in case of
 * success and an error in case `fopen` failed.
 */
E_Result
e_fs_file_open (E_File *file_out, const char *path, E_Fs_Open_Mode mode)
{
	FILE *handle;

	if (!file_out || !path) return E_ERR_INVALID_ARGUMENT;

	handle = fopen (path, mode_str_table[mode]);
	if (handle == nullptr) return E_RESULT_FROM_ERRNO ();

	file_out->handle = handle;
	return E_OK;
}

/**
 * Close the file handle \file. This function is essentially a wrapper around
 * `fclose`. Returns E_OK or the error that occured during `fclose`.
 */
void
e_fs_file_close (E_File *file)
{
	if (!file) return;
	fclose (file->handle);
}

/**
 * Get the total size of the file \file and store it in \size_out. Returns E_OK or an
 * error that occured during `fseek` or `ftell`.
 */
E_Result
e_fs_file_get_size (E_File *file, usize *size_out)
{
	long len, orig;
	int ret;

	if (!file || !size_out) return E_ERR_INVALID_ARGUMENT;

	orig = ftell (file->handle);
	if (orig < 0) goto err;

	ret = fseek (file->handle, 0L, SEEK_END);
	if (ret < 0) goto err;

	len = ftell (file->handle);
	if (ret < 0) goto err;

	ret = fseek (file->handle, orig, SEEK_SET);
	if (ret < 0) goto err;

	*size_out = (usize) len;
	return E_OK;
err:
	*size_out = 0;
	return E_RESULT_FROM_ERRNO ();
}

/**
 * Get the remaining size (i.e. from the current file position indicator to the
 * end of the file) of the file \file and store it in \size_out. Returns E_OK or
 * an error that occured during `fseek` or `ftell`.
 */
E_Result
e_fs_file_get_remaining_size (E_File *file, usize *size_out)
{
	long len, orig;
	int ret;

	if (!file || !size_out) return E_ERR_INVALID_ARGUMENT;

	orig = ftell (file->handle);
	if (orig < 0) goto err;

	ret = fseek (file->handle, 0L, SEEK_END);
	if (ret < 0) goto err;

	len = ftell (file->handle);
	if (ret < 0) goto err;

	ret = fseek (file->handle, orig, SEEK_SET);
	if (ret < 0) goto err;

	*size_out = (usize) len - (usize) orig;
	return E_OK;
err:
	*size_out = 0;
	return E_RESULT_FROM_ERRNO ();
}

/**
 * Get the file position indicator of the file \file and stores it in \pos. If
 * an error occurs, \pos is set to 0 and the error is returned.
 */
E_Result
e_fs_file_get_pos (E_File *file, usize *pos)
{
	long p;

	if (!file || !pos) return E_ERR_INVALID_ARGUMENT;

	p = ftell (file->handle);
	if (p < 0) {
		*pos = 0;
		return E_RESULT_FROM_ERRNO ();
	}

	*pos = (usize) p;
	return E_OK;
}

/**
 * Set the file position indicator of the file \file to \pos bytes after the
 * beginning of the file. Returns E_OK or an error that occured during `fseek`.
 */
E_Result
e_fs_file_set_pos (E_File *file, usize pos)
{
	int ret;

	if (!file || pos > LONG_MAX) return E_ERR_INVALID_ARGUMENT;

	ret = fseek (file->handle, (long) pos, SEEK_SET);
	if (ret < 0) return E_RESULT_FROM_ERRNO ();

	return E_OK;
}

/**
 * Rewind the file \file, i.e. set its position indicator to \pos bytes before
 * the end of the file. the file. Only returns an error if \file is `nullptr`.
 * Note that this behaves differently than a regular `fseek` with `SEEK_END` in
 * that `fseek` expects \pos to be negative, while this function expects it to
 * be positive.
 */
E_Result
e_fs_file_set_pos_end (E_File *file, usize pos)
{
	int ret;

	if (!file || pos > LONG_MAX) return E_ERR_INVALID_ARGUMENT;

	ret = fseek (file->handle, -((long) pos), SEEK_END);
	if (ret < 0) return E_RESULT_FROM_ERRNO ();

	return E_OK;
}

/**
 * Rewind the file \file, i.e. set its position indicator to the beginning of
 * the file. Only returns an error if \file is `nullptr`.
 */
E_Result
e_fs_file_rewind (E_File *file)
{
	if (!file) return E_ERR_INVALID_ARGUMENT;
	rewind (file->handle);
	return E_OK;
}

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
E_Result
e_fs_file_read (E_File *file, char *out, usize max_len, usize *len_out)
{
	usize count, offset;

	if (!file || !out) {
		if (out && max_len > 0) out[0] = 0;
		if (len_out) *len_out = 0;
		return E_ERR_INVALID_ARGUMENT;
	}

	if (max_len == 0) {
		if (len_out) *len_out = 0;
		return E_OK;
	}

	offset = 0;
	while (offset < max_len - 1) {
		clearerr (file->handle);
		count = fread (out + offset, sizeof (char),
		               max_len - offset - 1, file->handle);
		offset += count;
		if (count > 0) continue;
		if (feof (file->handle)) break;
		if (ferror (file->handle) != 0) {
			out[0] = 0;
			if (len_out) *len_out = 0;
			return E_ERR_FAIL;
		}
	}
	e_debug_assert (offset <= max_len - 1, "Read more from file than allowed");

	out[offset] = 0;
	if (len_out) *len_out = offset;
	return E_OK;
}

/**
 * Write \len bytes of data pointed to by \data to a file \file. Returns E_OK on
 * success, and an error on failure.
 */
E_Result
e_fs_file_write (E_File *file, const char *data, usize len)
{
	usize count, written;

	if (!file) return E_ERR_INVALID_ARGUMENT;
	if (!data || len == 0) return E_OK;

	written = 0;
	while (written < len) {
		clearerr (file->handle);
		count = fwrite (data + written, sizeof (char),
		                len - written, file->handle);
		written += count;
		if (count > 0) continue;
		if (ferror (file->handle) != 0) return E_ERR_FAIL;
	}

	return E_OK;
}

/**
 * Write a formatted string to the file handle \file. This uses the same format
 * as `fprintf` with \fmt being the format string. If \written is not `nullptr`,
 * the number of bytes written will be stored in \written. If an error occurs,
 * \written is set to 0.
 */
E_ATTR_FORMAT (printf, 3, 4)
E_Result e_fs_file_write_fmt (E_File *file, usize *written, const char *fmt, ...)
{
	va_list ap;
	int n;

	if (!file || !fmt) {
		if (written) *written = 0;
		return E_ERR_INVALID_ARGUMENT;
	}

	va_start (ap, fmt);
	n = vfprintf (file->handle, fmt, ap);
	va_end (ap);

	if (n < 0) {
		if (written) *written = 0;
		return E_ERR_FAIL;
	}

	if (written) *written = (usize) n;
	return E_OK;
}

/**
 * Check if a path exists.
 */
bool
e_fs_path_exists (const char *path)
{
	return access (path, F_OK) == 0;
}

/**
 * Check if a path points to a file.
 */
bool
e_fs_is_file (const char *path)
{
	struct stat s;
	stat (path, &s);
	return S_ISREG (s.st_mode);
}

/**
 * Check if a path points to a file.
 */
bool
e_fs_is_dir (const char *path)
{
	struct stat s;
	stat (path, &s);
	return S_ISDIR (s.st_mode);
}

/**
 * Check if a path points to a file.
 */
bool
e_fs_is_link (const char *path)
{
	struct stat s;
	stat (path, &s);
	return S_ISLNK (s.st_mode);
}

/**
 * Create a directory at path \path with permissions \perm.
 */
E_Result
e_fs_dir_create (const char *path, E_Fs_Permissions perm)
{
	int ret;

	if (!path) return E_ERR_INVALID_ARGUMENT;
	ret = mkdir (path, (mode_t) perm);
	if (ret < 0) return E_RESULT_FROM_ERRNO ();
	return E_OK;
}

#if E_CONFIG_MODULE_ALLOC

/**
 * Reads the entire content of the file with path \path into a buffer pointed to
 * by \out. The file will be opened and closed automatically. The behaviour and
 * arguments of this function are essentially the same as `e_fs_file_read_all`.
 */
E_Result
e_fs_read_file (const char *path, char **out, usize *len_out)
{
	E_Result res;
	E_File file;

	if (!path || !out) return E_ERR_INVALID_ARGUMENT;

	E_TRY (e_fs_file_open (&file, path, E_FS_OPEN_MODE_READ_ONLY));

	res = e_fs_file_read_all (&file, out, len_out);
	if (res != E_OK) {
		e_fs_file_close (&file);
		return res;
	}

	e_fs_file_close (&file);

	return E_OK;
}

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
 *  | E_Fs_File file;
 *  | char *buf;
 *  | usize len;
 *  | (void) e_fs_file_open (&file, "data.txt", E_FS_OPEN_MODE_READ_ONLY);
 *  | (void) e_fs_file_read_all (&file, &buf, &len);
 *  | e_fs_file_close (&file);
 *  | printf ("len: %zu, contents:\n%s\n", len, buf);
 *  | e_free (buf);
 *
 * Or, simply use the `e_fs_read_file()` wrapper instead of this boilerplate.
 */
E_Result
e_fs_file_read_all (E_File *file, char **out, usize *len_out)
{
	if (!file || !out) {
		if (out) *out = nullptr;
		if (len_out) *len_out = 0;
		return E_ERR_INVALID_ARGUMENT;
	}

	rewind (file->handle);

	return e_fs_file_read_remaining (file, out, len_out);
}

/**
 * Reads the remaining content (i.e. from the current file position indicator
 * to the end of the file) of the file handle \file into an allocated buffer.
 * The behaviour of this function is the same as `e_fs_file_read_all`, except
 * that it only reads the remaining content and not the entire file.
 */
E_Result
e_fs_file_read_remaining (E_File *file, char **out, usize *len_out)
{
	E_Result err;
	usize len;

	if (!file || !out) {
		if (out) *out = nullptr;
		if (len_out) *len_out = 0;
		return E_ERR_INVALID_ARGUMENT;
	}

	err = e_fs_file_get_size (file, &len);
	if (err != E_OK) {
		*out = nullptr;
		if (len_out) *len_out = 0;
		return err;
	}

	*out = e_alloc (char, len + 1);

	err = e_fs_file_read (file, *out, len + 1, len_out);
	if (err != E_OK) {
		e_free (*out);
		*out = nullptr;
		return err;
	}

	return E_OK;
}

#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_FS */
