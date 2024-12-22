#define E_INI_IMPL
#include <e_ini.h>

#include <e_test.h>
#include <e_types.h>

void
test_ini (void)
{
	E_Ini ini;

	const char *ini_str =
		"no_section = 123\n"
		"\n"
		"; comment\n"
		"[foo]\n"
		"  bar = baz \n"
		"qux =\n"
		"\n"
		"  [bar]  ; comment\n"
		"rab = zab\n";

	e_test_assert_ok ("e_ini_parse_str", e_ini_parse_str (&ini, ini_str));
	e_test_assert_str_eq ("e_ini_get_entry no_section", e_ini_get_entry (&ini, nullptr, "no_section")->value, "123");
	e_test_assert_str_eq ("e_ini_get_entry section", e_ini_get_entry (&ini, "bar", "rab")->value, "zab");
	e_test_assert_str_eq ("e_ini_get_entry empty", e_ini_get_entry (&ini, "foo", "qux")->value, "");
	e_test_assert_null ("e_ini_get_entry invalid section", e_ini_get_entry (&ini, "kjfadsf", "foo"));
	e_test_assert_null ("e_ini_get_entry invalid key", e_ini_get_entry (&ini, "foo", "kjfadsf"));
	e_ini_deinit (&ini);
}
