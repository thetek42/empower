#include "empower.h"

static const char *const mode_str_table[] = {
	"r", /* E_FS_OPEN_MODE_READ_ONLY */
	"w", /* E_FS_OPEN_MODE_WRITE_TRUNC */
	"a", /* E_FS_OPEN_MODE_WRITE_APPEND */
	"r+", /* E_FS_OPEN_MODE_READ_WRITE */
	"w+", /* E_FS_OPEN_MODE_READ_WRITE_TRUNC */
	"a+", /* E_FS_OPEN_MODE_READ_WRITE_APPEND */
};

e_result_t
e_fs_file_open (e_fs_file_t *file_out, const char *path, e_fs_open_mode_t mode)
{
	FILE *handle;

	if (!file_out || !path) return E_ERR_INVALID_ARGUMENT;

	handle = fopen (path, mode_str_table[mode]);
	if (handle == nullptr) {
		return (e_result_t) errno;
	}

	file_out->handle = handle;
	return E_OK;
}

e_result_t
e_fs_file_close (e_fs_file_t *file)
{
	int ret;

	if (!file) return E_ERR_INVALID_ARGUMENT;

	ret = fclose (file->handle);
	if (ret != 0) {
		return (e_result_t) errno;
	}

	return E_OK;
}

e_result_t
e_fs_file_get_size (e_fs_file_t *file, usize *size_out)
{
	long len;
	int ret;

	if (!file || !size_out) return E_ERR_INVALID_ARGUMENT;

	ret = fseek (file->handle, 0, SEEK_END);
	if (ret < 0) {
		*size_out = 0;
		return (e_result_t) errno;
	}

	len = ftell (file->handle);
	if (ret < 0) {
		*size_out = 0;
		return (e_result_t) errno;
	}

	ret = fseek (file->handle, 0, SEEK_SET);
	if (ret < 0) {
		*size_out = 0;
		return (e_result_t) errno;
	}

	*size_out = (usize) len;
	return E_OK;
}

e_result_t
e_fs_file_read_all (e_fs_file_t *file, char **out, usize *len_out)
{
	e_result_t err;
	usize len, count, offset;

	if (!file || !out) return E_ERR_INVALID_ARGUMENT;

	err = e_fs_file_get_size (file, &len);
	if (err != E_OK) goto err;

	*out = e_alloc (char, len + 1);
	offset = 0;
	while (offset < (usize) len) {
		clearerr (file->handle);
		count = fread (*out + offset, sizeof (char),
		               len - offset, file->handle);
		offset += count;
		if (count > 0) continue;
		if (feof (file->handle)) break;
		if (ferror (file->handle) != 0) {
			e_free (*out);
			err = E_ERR_FAIL;
			goto err;
		}
	}
	assert (offset <= (usize) len && "Read more from file than allowed");

	(*out)[offset] = 0;
	if (len_out) *len_out = offset;
	return E_OK;

err:
	*out = nullptr;
	if (len_out) *len_out = 0;
	return err;
}

e_result_t
e_fs_file_write (e_fs_file_t *file, const char *data, usize len)
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
