#ifndef _EMPOWER_INI_H_
#define _EMPOWER_INI_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_INI

/*! e_ini *********************************************************************
 * 
 * This module provides a simple INI parser.
 *
 * Module dependencies:
 *  - e_alloc
 *  - e_cstr
 *  - e_log
 *  - e_parse
 *  - e_result
 *  - e_vec
 *  - e_debug (transitive)
 *  - e_fs (optional)
 *
 * Example INI file:
 *  | [user]
 *  | name = admin
 *  | pass = password123
 *
 * Example code:
 *  | E_Ini ini;
 *  | E_TRY (e_ini_parse_file (&ini, "config.ini"))
 *  | E_Entry *entry = e_ini_get_entry (&ini, "section", "key");
 *  | if (entry) e_log_debug ("Entry value: ", entry->value);
 *  | e_ini_deinit (&ini);
 *
 ******************************************************************************/

#if !E_CONFIG_MODULE_ALLOC || !E_CONFIG_MODULE_CSTR || !E_CONFIG_MODULE_LOG || !E_CONFIG_MODULE_PARSE || !E_CONFIG_MODULE_RESULT || !E_CONFIG_MODULE_VEC
# error "module e_ini depends on: e_alloc, e_cstr, e_log, e_parse, e_result, e_vec"
#endif /* !E_CONFIG_MODULE_ALLOC || !E_CONFIG_MODULE_CSTR || !E_CONFIG_MODULE_LOG || !E_CONFIG_MODULE_PARSE || !E_CONFIG_MODULE_RESULT || !E_CONFIG_MODULE_VEC */

#include "e_compat.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_cstr.h"
#include "e_fs.h"
#include "e_log.h"
#include "e_parse.h"
#include "e_result.h"
#include "e_vec.h"

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

E_Result e_ini_parse_str (E_Ini *ini, const char *str);
void e_ini_deinit (E_Ini *ini);
E_Ini_Entry *e_ini_get_entry (E_Ini *ini, const char *section, const char *key);
void e_ini_debug (E_Ini *ini);

#if E_CONFIG_MODULE_FS
E_Result e_ini_parse_file (E_Ini *ini, const char *path);
#endif /* E_CONFIG_MODULE_FS */

#endif /* E_CONFIG_MODULE_INI */
#endif /* _EMPOWER_INI_H_ */
