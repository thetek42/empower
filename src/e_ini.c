#include "empower.h"

#if E_CONFIG_MODULE_INI

E_VEC_IMPLEMENT (E_Ini_Entry, __E_Vec_Ini_Entry, __e_vec_ini_entry);
E_VEC_IMPLEMENT (E_Ini_Section, __E_Vec_Ini_Section, __e_vec_ini_section);

static E_Result __e_ini_parse_str (E_Ini *ini, const char *str, usize len);

E_Result
e_ini_parse_str (E_Ini *ini, const char *str)
{
	return __e_ini_parse_str (ini, str, strlen (str));
}

#if E_CONFIG_MODULE_FS

E_Result
e_ini_parse_file (E_Ini *ini, const char *path)
{
	E_Result res;
	usize len;
	char *buf;

	E_TRY (e_fs_read_file (path, &buf, &len));

	res = __e_ini_parse_str (ini, buf, len);
	if (res != E_OK) goto ret;

	res = E_OK;
ret:
	e_free (buf);
	return res;
}

#endif /* E_CONFIG_MODULE_FS */

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
	E_Ini_Entry entry;
	const char *s, *token_start, *key;
	char *str_buf, *str_buf_end;
	usize i, line, token_len, key_len;

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

	s = str;
	line = 0;
	str_buf_end = str_buf;
	
	while (*s != 0) {
		e_parse_consume_whitespace_until_newline (&s);

		/* newline */
		if (e_parse_char (&s, '\n')) {
			line += 1;
			continue;
		}

		/* comments */
		if (e_parse_char (&s, ';')) {
			e_parse_consume_line (&s);
			continue;
		}

		/* namespace */
		if (e_parse_char (&s, '[')) {
			if (*s == 0) goto unexpected_eof;
			token_start = e_parse_consume_until_pat (&s, "]\t\v\f\r\n");
			if (*s == 0) goto unexpected_eof;
			token_len = s - token_start;
			if (token_len == 0) {
				e_log_error ("ini parsing failed (line %zu): namespace name cannot be empty", line);
				goto err;
			}
			if (!e_parse_char (&s, ']')) {
				e_log_error ("ini parsing failed (line %zu): expected ] after namespace name", line);
				goto err;
			}
			e_parse_consume_whitespace_until_newline (&s);
			if (*s == ';') {
				e_parse_consume_line (&s);
			} else if (*s != 0 && *s != '\n') {
				e_log_error ("ini parsing failed (line %zu): unexpected character after namespace declaration", line);
				goto err;
			}

			for (i = 0; i < sections.len; i++) {
				if (!sections.ptr[i].name) continue;
				if (strcmp (sections.ptr[i].name, token_start) == 0) {
					e_log_error ("ini parsing failed (line %zu): duplicate section", line);
					goto err;
				}
			}

			__e_vec_ini_section_append (&sections, (E_Ini_Section) {
				.name = str_buf_end,
				.entries_idx = entries.len,
				.entries_len = 0,
			});

			strncpy (str_buf_end, token_start, token_len);
			str_buf_end[token_len] = 0;
			str_buf_end += token_len + 1;

			continue;
		}

		/* entry key */
		key = e_parse_consume_until_pat (&s, "=\r\n");
		if (*s == 0) goto unexpected_eof;
		if (!e_parse_char (&s, '=')) {
			e_log_error ("ini parsing failed (line %zu): expected = after entry key", line);
			goto err;
		}
		key_len = s - key - 1;
		while (key_len > 0 && isspace (key[key_len - 1])) key_len -= 1;
		e_parse_consume_whitespace_until_newline (&s);

		/* entry value */
		token_start = e_parse_consume_line (&s);
		token_len = s - token_start;
		while (token_len > 0 && isspace (token_start[token_len - 1])) token_len -= 1;
		
		entry.key = str_buf_end;
		strncpy (str_buf_end, key, key_len);
		str_buf_end[key_len] = 0;
		str_buf_end += key_len + 1;
		entry.value = str_buf_end;
		strncpy (str_buf_end, token_start, token_len);
		str_buf_end[token_len] = 0;
		str_buf_end += token_len + 1;
		__e_vec_ini_entry_append (&entries, entry);
		sections.ptr[sections.len - 1].entries_len += 1;
	}

	ini->sections = sections;
	ini->entries = entries;
	ini->str_buf = str_buf;
	return E_OK;

unexpected_eof:
	e_log_error ("ini parsing failed (line %zu): unexpected eof", line);
err:
	e_free (str_buf);
	__e_vec_ini_entry_deinit (&entries);
	__e_vec_ini_section_deinit (&sections);
	return E_ERR_FORMAT_ERROR;
}

void
e_ini_debug (E_Ini *ini)
{
	E_Ini_Entry *entries;
	usize si, ei;

	if (!ini) return;

	fprintf (stderr, "\n---=== Debugging INI %p ===---", (void *) ini);
	for (si = 0; si < ini->sections.len; si++) {
		fprintf (stderr, "[%s]\n", ini->sections.ptr[si].name);
		entries = &ini->entries.ptr[ini->sections.ptr[si].entries_idx];
		for (ei = 0; ei < ini->sections.ptr[si].entries_len; ei++) {
			fprintf (stderr, "  %s = %s\n",
		                 entries[ei].key, entries[ei].value);
		}
	}
	fprintf (stderr, "\n");
}

#endif /* E_CONFIG_MODULE_INI */
