#ifndef _EMPOWER_INI_H_
#define _EMPOWER_INI_H_

/*! e_ini *********************************************************************
 * 
 * This module provides a simple INI parser.
 *
 * Module dependencies:
 *  - e_parse
 *  - e_result
 *  - e_vec
 *
 * Example INI file:
 *  | [user]
 *  | name = admin
 *  | pass = password123
 *
 * Example code:
 *  | #define E_PARSE_IMPL
 *  | #define E_RESULT_IMPL
 *  | #include <e_ini.h>
 *  |
 *  | E_Ini ini;
 *  | E_TRY (e_ini_parse_file (&ini, "config.ini"))
 *  | E_Entry *entry = e_ini_get_entry (&ini, "section", "key");
 *  | if (entry) e_log_debug ("Entry value: ", entry->value);
 *  | e_ini_deinit (&ini);
 *
 ******************************************************************************/

#include <stddef.h>
#include <e_parse.h>
#include <e_result.h>
#include <e_vec.h>

/* public interface ***********************************************************/

/* TODO: ini format documentation */
/* TODO: multiline values */
/* TODO: tests, tests, tests */
/* TODO: entry value parsing (e.g. for integers) */

/**
 * An entry within an INI file, consisting of a nul-terminated \key and a
 * nul-terminated \value.
 */
typedef struct {
	const char *key;
	const char *value;
} E_Ini_Entry;

/**
 * A section within an INI file, consisting of a nul-terminated \name and some
 * entries. \entries_idx is the index of the first entry within the `entries`
 * field of the `E_Ini` struct, and \entries_len is the number of entries in
 * this section.
 */
typedef struct {
	const char *name;
	size_t entries_idx;
	size_t entries_len;
} E_Ini_Section;

E_VEC_DECL (E_Ini_Entry, E_Vec_Ini_Entry, e_vec_ini_entry);
E_VEC_DECL (E_Ini_Section, E_Vec_Ini_Section, e_vec_ini_section);

/**
 * A parsed INI file.
 */
typedef struct {
	char *str_buf;
	E_Vec_Ini_Entry entries;
	E_Vec_Ini_Section sections;
} E_Ini;

E_Result e_ini_parse_str (E_Ini *ini, const char *str);
void e_ini_deinit (E_Ini *ini);
E_Ini_Entry *e_ini_get_entry (E_Ini *ini, const char *section, const char *key);
void e_ini_debug (E_Ini *ini);

/* implementation *************************************************************/

#ifdef E_INI_IMPL

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

E_VEC_IMPL (E_Ini_Entry, E_Vec_Ini_Entry, e_vec_ini_entry);
E_VEC_IMPL (E_Ini_Section, E_Vec_Ini_Section, e_vec_ini_section);

static E_Result e_ini_priv_parse_str (E_Ini *ini, const char *str, size_t len);

/**
 * Parse an INI file from a string \str. The parsed file will be stored in \ini.
 * A result is returned, indicating whether the parsing was successful or not.
 * In case of a parsing error, the error is printed using `e_log_error`.
 */
E_Result
e_ini_parse_str (E_Ini *ini, const char *str)
{
	return e_ini_priv_parse_str (ini, str, strlen (str));
}

/**
 * Deinitialise the parsed INI file \ini and free its occupied memory.
 */
void
e_ini_deinit (E_Ini *ini)
{
	if (!ini) return;
	free (ini->str_buf);
	e_vec_ini_entry_deinit (&ini->entries);
	e_vec_ini_section_deinit (&ini->sections);
}

/**
 * Obtain an entry within the INI file \ini, located in the section \section
 * with the key \key. Entries that are not stored in a section can be retrieved
 * using `nullptr` for \section. If no entry is found or \ini or \key are
 * `nullptr`, `nullptr` is returned.
 */
E_Ini_Entry *
e_ini_get_entry (E_Ini *ini, const char *section, const char *key)
{
	E_Ini_Entry *entries;
	size_t i, num_entries;
	const char *name;

	if (!ini || !key) return NULL;

	for (i = 0; i < ini->sections.len; i++) {
		name = ini->sections.ptr[i].name;
		if ((!section && name) || (section && !name)) continue;
		if (section && name && strcmp (section, name)) continue;
		entries = ini->entries.ptr + ini->sections.ptr[i].entries_idx;
		num_entries = ini->sections.ptr[i].entries_len;
		for (i = 0; i < num_entries; i++) {
			if (strcmp (entries[i].key, key)) continue;
			return &entries[i];
		}
		return NULL;
	}

	return NULL;
}

/**
 * Debug print a parsed INI file.
 */
static E_Result
e_ini_priv_parse_str (E_Ini *ini, const char *str, size_t len)
{
	E_Vec_Ini_Section sections;
	E_Vec_Ini_Entry entries;
	E_Ini_Entry entry;
	const char *s, *token_start, *key;
	char *str_buf, *str_buf_end;
	size_t i, line, token_len, key_len;

	if (!ini || !str) return E_ERR_INVALID_ARGUMENT;

	sections = e_vec_ini_section_init ();
	entries = e_vec_ini_entry_init ();
	str_buf = malloc (sizeof (char) * (len + 1));
        if (!str_buf) {
		fprintf (stderr, "[e_ini] failed to alloc %zu bytes\n", len + 1);
		exit (EXIT_FAILURE);
	}
        /* NOTE: str_buf will never need to be realloced since it contains
         * enough space to store everything. */

	e_vec_ini_section_append (&sections, (E_Ini_Section) {
		.name = NULL,
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
				fprintf (stderr, "[e_ini] parsing failed (line %zu): namespace name cannot be empty\n", line);
				goto err;
			}
			if (!e_parse_char (&s, ']')) {
				fprintf (stderr, "[e_ini] parsing failed (line %zu): expected ] after namespace name\n", line);
				goto err;
			}
			e_parse_consume_whitespace_until_newline (&s);
			if (*s == ';') {
				e_parse_consume_line (&s);
			} else if (*s != 0 && *s != '\n') {
				fprintf (stderr, "[e_ini] parsing failed (line %zu): unexpected character after namespace declaration\n", line);
				goto err;
			}

			for (i = 0; i < sections.len; i++) {
				if (!sections.ptr[i].name) continue;
				if (strcmp (sections.ptr[i].name, token_start) == 0) {
					fprintf (stderr, "[e_ini] parsing failed (line %zu): duplicate section\n", line);
					goto err;
				}
			}

			e_vec_ini_section_append (&sections, (E_Ini_Section) {
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
			fprintf (stderr, "[e_ini] parsing failed (line %zu): expected = after entry key\n", line);
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
		e_vec_ini_entry_append (&entries, entry);
		sections.ptr[sections.len - 1].entries_len += 1;
	}

	ini->sections = sections;
	ini->entries = entries;
	ini->str_buf = str_buf;
	return E_OK;

unexpected_eof:
	fprintf (stderr, "[e_ini] parsing failed (line %zu): unexpected eof\n", line);
err:
	free (str_buf);
	e_vec_ini_entry_deinit (&entries);
	e_vec_ini_section_deinit (&sections);
	return E_ERR_FORMAT_ERROR;
}

void
e_ini_debug (E_Ini *ini)
{
	E_Ini_Entry *entries;
	size_t si, ei;

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

#endif /* E_INI_IMPL */

#endif /* _EMPOWER_INI_H_ */
