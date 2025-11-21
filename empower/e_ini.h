#ifndef EMPOWER_INI_H_
#define EMPOWER_INI_H_

/******************************************************************************
 *
 * A simple callback-based INI parser.
 *
 * Configuration options:
 *  - `E_CONFIG_INI_MAX_CATEGORY_LEN` (default 64)
 *  - `E_CONFIG_INI_MAX_KEY_LEN` (default 64)
 *  - `E_CONFIG_INI_MAX_VALUE_LEN` (default 256)
 *
 ******************************************************************************/

#include <stdio.h>
typedef int (*e_ini_callback_t) (const char *cateogry,
                                 const char *key,
                                 const char *value,
                                 void *user);

int e_ini_parse (const char *str,
                 e_ini_callback_t callback,
                 void *user);

/******************************************************************************/

#ifdef E_INI_IMPL

#include <stddef.h>

#ifndef E_CONFIG_INI_MAX_CATEGORY_LEN
# define E_CONFIG_INI_MAX_CATEGORY_LEN 64
#endif
#ifndef E_CONFIG_INI_MAX_KEY_LEN
# define E_CONFIG_INI_MAX_KEY_LEN 64
#endif
#ifndef E_CONFIG_INI_MAX_VALUE_LEN
# define E_CONFIG_INI_MAX_VALUE_LEN 256
#endif

static int e_ini_parse_category_ (const char **str, char *category);
static int e_ini_parse_key_ (const char **str, char *key);
static int e_ini_parse_value_ (const char **str, char *value);

/**
 * Parse an INI file.
 *
 * `str` is the string that should be parsed. For every key, `callback` will be
 * called with the following parameters:
 *   - category: The category of the key as a nul-terminated string or NULL
 *   - key: The key as a nul-terminated string
 *   - value: The value as a nul-terminated string
 *   - user: The user data passed to `user` in this function
 *
 * When the callback returns non-zero, this indicates an error. Parsing will
 * stop and the error that was returned from the callback will be returned to
 * the user.
 *
 * The function returns zero on success. If the callback returned an error or
 * the parsing failed because of an invalid syntax, non-zero is returned. The
 * returned value is the line number at which parsing failed, where the first
 * line will be counted as 1.
 */
int
e_ini_parse (const char *str,
             e_ini_callback_t callback,
             void *user)
{
        char category[E_CONFIG_INI_MAX_CATEGORY_LEN];
        char key[E_CONFIG_INI_MAX_KEY_LEN];
        char value[E_CONFIG_INI_MAX_VALUE_LEN];
        char *category_arg = NULL;
        char c;
        int ret;
        int lineno = 1;

        while ((c = *str) != '\0') {
                switch (c) {
                case ' ':
                case '\t':
                case '\r':
                        break;
                case '\n':
                        lineno += 1;
                        break;
                case '[':
                        str++;
                        ret = e_ini_parse_category_ (&str, category);
                        if (ret != 0) return lineno;
                        category_arg = category;
                        break;
                default:
                        ret = e_ini_parse_key_ (&str, key);
                        if (ret != 0) return lineno;
                        ret = e_ini_parse_value_ (&str, value);
                        if (ret != 0) return lineno;
                        ret = callback (category_arg, key, value, user);
                        if (ret != 0) return lineno;
                        break;
                }
                str++;
        }

        return 0;
}

static int
e_ini_parse_category_ (const char **str, char *category)
{
        int i;
        char c;
        for (i = 0; i < E_CONFIG_INI_MAX_CATEGORY_LEN - 1; i++) {
                c = **str;
                switch (c) {
                case '\n':
                case '\r':
                case '\0':
                        return 1;
                case ']':
                        category[i] = 0;
                        return 0;
                default:
                        category[i] = c;
                        break;
                }
                (*str)++;
        }
        return 1;
}

static int
e_ini_parse_key_ (const char **str, char *key)
{
        int i, last_non_space;
        char c;
        last_non_space = 0;
        for (i = 0; i < E_CONFIG_INI_MAX_KEY_LEN - 1; i++) {
                c = **str;
                switch (c) {
                case '\n':
                case '\r':
                case '\0':
                        return 1;
                case '=':
                        key[last_non_space + 1] = 0;
                        (*str)++;
                        return 0;
                case ' ':
                case '\t':
                        key[i] = c;
                        break;
                default:
                        key[i] = c;
                        last_non_space = i;
                        break;
                }
                (*str)++;
        }
        return 1;
}

static int
e_ini_parse_value_ (const char **str, char *value)
{
        int i, last_non_space;
        char c;
        last_non_space = 0;
        while ((c = **str)) {
                if (c != ' ' && c != '\t') break;
                (*str)++;
        }
        for (i = 0; i < E_CONFIG_INI_MAX_VALUE_LEN - 1; i++) {
                c = **str;
                switch (c) {
                case '\n':
                case '\r':
                case '\0':
                        value[last_non_space + 1] = 0;
                        return 0;
                case ' ':
                case '\t':
                        value[i] = c;
                        break;
                default:
                        value[i] = c;
                        last_non_space = i;
                        break;
                }
                (*str)++;
        }
        return 1;
}

#endif /* E_INI_IMPL */

#endif /* EMPOWER_INI_H_ */
