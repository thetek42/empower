#include "empower.h"

#if E_CONFIG_MODULE_INI

E_VEC_IMPLEMENT (E_Ini_Entry, __E_Vec_Ini_Entry, __e_vec_ini_entry);
E_VEC_IMPLEMENT (E_Ini_Section, __E_Vec_Ini_Section, __e_vec_ini_section);

static E_Result __e_ini_parse_str (E_Ini *ini, const char *str, usize len);
static int is_rbacket_or_nl (int c);
static int is_eq_or_nl (int c);

E_Result
e_ini_parse_str (E_Ini *ini, const char *str)
{
	return __e_ini_parse_str (ini, str, strlen (str));
}

E_Result
e_ini_parse_file (E_Ini *ini, const char *path)
{
	E_Fs_File file;
	E_Result res;
	usize len;
	char *buf;

	res = e_fs_file_open (&file, path, E_FS_OPEN_MODE_READ_ONLY);
	if (res != E_OK) return res;

	res = e_fs_file_read_all (&file, &buf, &len);
	if (res != E_OK) return res;

	res = e_fs_file_close (&file);
	if (res != E_OK) goto ret;

	res = __e_ini_parse_str (ini, buf, len);
	if (res != E_OK) goto ret;

	res = E_OK;
ret:
	e_free (buf);
	return res;
}

void
e_ini_deinit (E_Ini *ini)
{
	if (!ini) return;
	e_free (ini->str_buf);
	__e_vec_ini_entry_deinit (&ini->entries);
	__e_vec_ini_section_deinit (&ini->sections);
}

E_Ini_Entry *
e_ini_get_entry (E_Ini *ini, const char *section, const char *key)
{
	E_Ini_Entry *entries;
	usize i, num_entries;

	if (!ini || !key) return nullptr;

	for (i = 0; i < ini->sections.len; i++) {
		if (!e_cstr_eq (ini->sections.ptr[i].name, section)) continue;
		entries = ini->entries.ptr + ini->sections.ptr[i].entries_idx;
		num_entries = ini->sections.ptr[i].entries_len;
		for (i = 0; i < num_entries; i++) {
			if (!e_cstr_eq (entries[i].key, key)) continue;
			return &entries[i];
		}
		return nullptr;
	}

	return nullptr;
}

static E_Result
__e_ini_parse_str (E_Ini *ini, const char *str, usize len)
{
	__E_Vec_Ini_Section sections;
	__E_Vec_Ini_Entry entries;
	const char *end, *eq, *section_name, *key;
	char *str_buf, *str_buf_end;
	usize line, i;

	if (!ini || !str) return E_ERR_INVALID_ARGUMENT;

	sections = __e_vec_ini_section_init ();
	entries = __e_vec_ini_entry_init ();
	str_buf = e_alloc (char, len); /* NOTE: str_buf will never need to be
	realloced since it contains enough space to store everything. */

	__e_vec_ini_section_append (&sections, (E_Ini_Section) {
		.name = nullptr,
		.entries_idx = 0,
		.entries_len = 0,
	});

	str_buf_end = str_buf;
	section_name = nullptr;
	line = 1;
	while (len > 0) {

		if (isspace (*str)) {
			if (*str == '\n') line += 1;
			str++;
			len--;
			continue;
		}

		if (*str == '[') {
			end = e_cstr_find_char_func (str, is_rbacket_or_nl);
			if (!end || *end != ']') {
				e_log_error ("ini: unexpected eof (expected `]`) in line %zu", line);
				goto err;
			}
			if (*end != ']') {
				e_log_error ("ini: unexpected newline (expected `]`) in line %zu", line);
				goto err;
			}
			section_name = str_buf_end;
			strncpy (str_buf_end, str + 1, end - str - 1);
			str_buf_end[end - str - 1] = 0;
			str_buf_end += end - str;
			for (i = 0; i < sections.len; i++) {
				if (!sections.ptr[i].name) continue;
				if (e_cstr_eq (sections.ptr[i].name, section_name)) {
					e_log_error ("ini: duplicate section in line %zu", line);
					goto err;
				}
			}
			__e_vec_ini_section_append (&sections, (E_Ini_Section) {
				.name = section_name,
				.entries_idx = entries.len,
				.entries_len = 0,
			});
			len -= end - str + 1;
			str = end + 1;
			while (len > 0 && *str != '\n' && isspace (*str)) {
				str += 1;
				len -= 1;
			}
			if (len == 0) break;
			if (*str != '\n') {
				e_log_error ("ini: unexpected character %c in line %zu", *str, line);
				goto err;
			}
			continue;
		}

		if (*str == '=') {
			e_log_error ("ini: unexpected = in line %zu", line);
			goto err;
		}

		end = e_cstr_find_char_func (str, is_eq_or_nl);
		if (!end) {
			e_log_error ("ini: unexpected eof (expected `=`) in line %zu", line);
			goto err;
		}
		if (*end != '=') {
			e_log_error ("ini: unexpected newline (expected `=`) in line %zu", line);
			goto err;
		}
		eq = end;
		for (end -= 1; end > str && isspace (*end); end--) {}
		key = str_buf_end;
		strncpy (str_buf_end, str, end - str + 1);
		str_buf_end[end - str + 1] = 0;
		str_buf_end += end - str + 2;
		len -= eq - str + 1;
		str = eq + 1;
		while (len > 0 && *str != '\n' && isspace (*str)) {
			str += 1;
			len -= 1;
		}
		if (*str == '\n') {
			*str_buf_end = 0;
			__e_vec_ini_entry_append (&entries, (E_Ini_Entry) {
				.key = key,
				.value = str_buf_end,
			});
			sections.ptr[sections.len - 1].entries_len += 1;
			str_buf_end += 1;
			continue;
		} else {
			end = strchr (str, '\n');
			if (!end) end = &str[len];
			for (end -= 1; end > str && isspace (*end); end--) {}
			strncpy (str_buf_end, str, end - str + 1);
			str_buf_end[end - str + 1] = 0;
			__e_vec_ini_entry_append (&entries, (E_Ini_Entry) {
				.key = key,
				.value = str_buf_end,
			});
			sections.ptr[sections.len - 1].entries_len += 1;
			str_buf_end += end - str + 2;
			len -= end - str + 1;
			str = end + 1;
			continue;
		}
	}

	ini->sections = sections;
	ini->entries = entries;
	ini->str_buf = str_buf;
	return E_OK;
err:
	e_free (str_buf);
	__e_vec_ini_entry_deinit (&entries);
	__e_vec_ini_section_deinit (&sections);
	return E_ERR_INVALID_FORMAT;
}

static int
is_rbacket_or_nl (int c)
{
	return c == ']' || c == '\n' || c == '\r';
}

static int
is_eq_or_nl (int c)
{
	return c == '=' || c == '\n' || c == '\r';
}

#endif /* E_CONFIG_MODULE_INI */
