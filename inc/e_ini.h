#ifndef _EMPOWER_INI_H_
#define _EMPOWER_INI_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_INI

#if !E_CONFIG_MODULE_CSTR || !E_CONFIG_MODULE_LOG || !E_CONFIG_MODULE_PARSE || !E_CONFIG_MODULE_RESULT || !E_CONFIG_MODULE_VEC
# error "module e_ini depends on: e_cstr, e_log, e_parse, e_result, e_vec"
#endif /* !E_CONFIG_MODULE_CSTR || !E_CONFIG_MODULE_LOG || !E_CONFIG_MODULE_PARSE || !E_CONFIG_MODULE_RESULT || !E_CONFIG_MODULE_VEC */

/*! e_ini *********************************************************************
 * 
 * This module provides a simple INI parser. Example INI file:
 *
 * | [user]
 * | name = admin
 * | pass = password123
 *
 * Module dependencies:
 *  - e_alloc
 *  - e_cstr
 *  - e_log
 *  - e_parse
 *  - e_result
 *  - e_vec
 *  - e_fs (optional)
 *
 ******************************************************************************/

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
	usize entries_idx;
	usize entries_len;
} E_Ini_Section;

E_VEC_DECLARE (E_Ini_Entry, __E_Vec_Ini_Entry, __e_vec_ini_entry);
E_VEC_DECLARE (E_Ini_Section, __E_Vec_Ini_Section, __e_vec_ini_section);

/**
 * A parsed INI file.
 */
typedef struct {
	char *str_buf;
	__E_Vec_Ini_Entry entries;
	__E_Vec_Ini_Section sections;
} E_Ini;

/**
 * Parse an INI file from a string \str. The parsed file will be stored in \ini.
 * A result is returned, indicating whether the parsing was successful or not.
 * In case of a parsing error, the error is printed using `e_log_error`.
 */
E_Result e_ini_parse_str (E_Ini *ini, const char *str);

#if E_CONFIG_MODULE_FS

/**
 * Parse an INI file from a file located at \path. The parsed file will be
 * stored in \ini. A result is returned, indicating whether the parsing was
 * successful or not. In case of a parsing error, the error is printed using
 * `e_log_error`.
 */
E_Result e_ini_parse_file (E_Ini *ini, const char *path);

#endif /* E_CONFIG_MODULE_FS */

/**
 * Deinitialise the parsed INI file \ini and free its occupied memory.
 */
void e_ini_deinit (E_Ini *ini);

/**
 * Obtain an entry within the INI file \ini, located in the section \section
 * with the key \key. Entries that are not stored in a section can be retrieved
 * using `nullptr` for \section. If no entry is found or \ini or \key are
 * `nullptr`, `nullptr` is returned.
 */
E_Ini_Entry *e_ini_get_entry (E_Ini *ini, const char *section, const char *key);

/**
 * Debug print a parsed INI file.
 */
void e_ini_debug (E_Ini *ini);

#endif /* E_CONFIG_MODULE_INI */
#endif /* _EMPOWER_INI_H_ */
