#include "empower.h"

#if E_CONFIG_MODULE_FS

static const char *const mode_str_table[] = {
	"r", /* E_FS_OPEN_MODE_READ_ONLY */
	"w", /* E_FS_OPEN_MODE_WRITE_TRUNC */
	"a", /* E_FS_OPEN_MODE_WRITE_APPEND */
	"r+", /* E_FS_OPEN_MODE_READ_WRITE */
	"w+", /* E_FS_OPEN_MODE_READ_WRITE_TRUNC */
	"a+", /* E_FS_OPEN_MODE_READ_WRITE_APPEND */
};

E_ATTR_NODISCARD ("E_Result must be checked")
E_Result
e_fs_read_file (const char *path, char **out, usize *len_out)
{
	E_Result res;
	E_File file;

	if (!path || !out) return E_ERR_INVALID_ARGUMENT;

	res = e_fs_file_open (&file, path, E_FS_OPEN_MODE_READ_ONLY);
	if (res != E_OK) return res;

	res = e_fs_file_read_all (&file, out, len_out);
	if (res != E_OK) {
		e_fs_file_close (&file);
		return res;
	}

	e_fs_file_close (&file);

	return E_OK;
}

E_ATTR_NODISCARD ("E_Result must be checked")
E_Result
e_fs_file_open (E_File *file_out, const char *path, E_Fs_Open_Mode mode)
{
	FILE *handle;

	if (!file_out || !path) return E_ERR_INVALID_ARGUMENT;

	handle = fopen (path, mode_str_table[mode]);
	if (handle == nullptr) {
		return (E_Result) errno;
	}

	file_out->handle = handle;
	return E_OK;
}

void
e_fs_file_close (E_File *file)
{
	if (!file) return;
	fclose (file->handle);
}

E_ATTR_NODISCARD ("E_Result must be checked")
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
	return (E_Result) errno;
}

E_ATTR_NODISCARD ("E_Result must be checked")
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
	return (E_Result) errno;
}

E_ATTR_NODISCARD ("E_Result must be checked")
E_Result
e_fs_file_get_pos (E_File *file, usize *pos)
{
	long p;

	if (!file || !pos) return E_ERR_INVALID_ARGUMENT;

	p = ftell (file->handle);
	if (p < 0) {
		*pos = 0;
		return (E_Result) errno;
	}

	*pos = (usize) p;
	return E_OK;
}

E_ATTR_NODISCARD ("E_Result must be checked")
E_Result
e_fs_file_set_pos (E_File *file, usize pos)
{
	int ret;

	if (!file || pos > LONG_MAX) return E_ERR_INVALID_ARGUMENT;

	ret = fseek (file->handle, (long) pos, SEEK_SET);
	if (ret < 0) return (E_Result) errno;

	return E_OK;
}

E_ATTR_NODISCARD ("E_Result must be checked")
E_Result
e_fs_file_set_pos_end (E_File *file, usize pos)
{
	int ret;

	if (!file || pos > LONG_MAX) return E_ERR_INVALID_ARGUMENT;

	ret = fseek (file->handle, -((long) pos), SEEK_END);
	if (ret < 0) return (E_Result) errno;

	return E_OK;
}

E_ATTR_NODISCARD ("E_Result must be checked")
E_Result
e_fs_file_rewind (E_File *file)
{
	if (!file) return E_ERR_INVALID_ARGUMENT;
	rewind (file->handle);
	return E_OK;
}

E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_read (E_File *file, char *out, usize max_len, usize *len_out)
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
	assert (offset <= max_len - 1 && "Read more from file than allowed");

	out[offset] = 0;
	if (len_out) *len_out = offset;
	return E_OK;
}

#if E_CONFIG_MODULE_ALLOC

E_ATTR_NODISCARD ("E_Result must be checked")
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

E_ATTR_NODISCARD ("E_Result must be checked")
E_Result e_fs_file_read_remaining (E_File *file, char **out, usize *len_out)
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

E_ATTR_NODISCARD ("E_Result must be checked")
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

E_ATTR_FORMAT (printf, 3, 4)
E_ATTR_NODISCARD ("E_Result must be checked")
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

bool
e_fs_path_exists (const char *path)
{
	return access (path, F_OK) == 0;
}

bool
e_fs_is_file (const char *path)
{
	struct stat s;
	stat (path, &s);
	return S_ISREG (s.st_mode);
}

bool
e_fs_is_dir (const char *path)
{
	struct stat s;
	stat (path, &s);
	return S_ISDIR (s.st_mode);
}

bool
e_fs_is_link (const char *path)
{
	struct stat s;
	stat (path, &s);
	return S_ISLNK (s.st_mode);
}

#endif /* E_CONFIG_MODULE_FS */
