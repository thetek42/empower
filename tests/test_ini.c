#define E_INI_IMPL
#include <e_ini.h>

#include <e_test.h>

#include <stdio.h>
#include <string.h>

static int test_number = 0;

static int
callback (const char *category,
          const char *key,
          const char *value,
          void *user)
{
        char test_name[32];

        (void) user;
        sprintf (test_name, "e_ini_parse %d", test_number);
        test_number += 1;

        if (category == NULL) {
                if (strcmp (key, "foo") == 0) {
                        e_test_assert_str_eq (test_name, value, "bar");
                } else if (strcmp (key, "foo bar") == 0) {
                        e_test_assert_str_eq (test_name, value, "baz qux");
                } else {
                        e_test_assert (test_name, 0);
                }
        } else if (strcmp (category, "category") == 0) {
                if (strcmp (key, "bar") == 0) {
                        e_test_assert_str_eq (test_name, value, "foo");
                } else if (strcmp (key, "bar baz") == 0) {
                        e_test_assert_str_eq (test_name, value, "foo qux");
                } else {
                        e_test_assert (test_name, 0);
                }
        } else if (strcmp (category, "other") == 0) {
                if (strcmp (key, "baz") == 0) {
                        e_test_assert_str_eq (test_name, value, "qux");
                } else if (strcmp (key, "baz qux") == 0) {
                        e_test_assert_str_eq (test_name, value, "foo bar");
                } else {
                        e_test_assert (test_name, 0);
                }
        } else {
                e_test_assert (test_name, 0);
        }

        return 0;
}

static const char *const ini =
        "foo=bar\n"
        "  foo=bar\n"
        "foo  =bar\n"
        "foo=  bar\n"
        "foo=bar  \n"
        "  foo  =  bar  \n"
        "  foo  =  bar  ;comment \n"
        "foo bar=baz qux\n"
        "  foo bar  =  baz qux  \n"
        "\n"
        "[category] ;comment\n"
        "bar=foo\n"
        "bar baz=foo qux\n"
        "  ; comment\n"
        "\n"
        "[other]\n"
        "baz=qux\n"
        "baz qux=foo bar\n";

void
test_ini (void)
{
        int ret;
        ret = e_ini_parse (ini, callback, NULL);
        e_test_assert_eq ("e_ini_parse ret", int, ret, 0);
}
