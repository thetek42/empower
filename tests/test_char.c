#define E_CHAR_IMPL
#include "e_char.h"
#include "e_test.h"

static void test_func (const char *name,
                       int (*func) (char),
                       const char *accept,
                       const char *reject,
                       int accept_nul,
                       int accept_nonascii);

void
test_char (void)
{
    const char ascii_bounds[] = {0x01, 0x7F, 0x00};

    test_func ("e_char_isalnum", e_char_isalnum, "0189abyzABYZ", "\n /:?@_`{", 0, 0);
    test_func ("e_char_isalpha", e_char_isalpha, "abyzABYZ", "0189\n /:?@_`{", 0, 0);
    test_func ("e_char_isascii", e_char_isascii, ascii_bounds, "", 1, 0);
    test_func ("e_char_isblank", e_char_isblank, " \t", "\naA0", 0, 0);
    test_func ("e_char_iscntrl", e_char_iscntrl, "\n\r\t\v\f", " aA0", 1, 0);
    test_func ("e_char_isdigit", e_char_isdigit, "0189", "\n /:?@_`{", 0, 0);
    test_func ("e_char_isgraph", e_char_isgraph, "0189abyzABYZ/:?@_`{", " \n\r\t", 0, 0);
    test_func ("e_char_islower", e_char_islower, "abyz", "0189ABYZ/:?@_`{", 0, 0);
    test_func ("e_char_isprint", e_char_isprint, "0189abyzABYZ/:?@_`{ ", "\n\r\t", 0, 0);
    test_func ("e_char_ispunct", e_char_ispunct, "/:?@_`{", "0189abyzABYZ", 1, 1);
    test_func ("e_char_isspace", e_char_isspace, " \n\r\t\v\f", "aA0.", 0, 0);
    test_func ("e_char_isupper", e_char_isupper, "ABYZ", "0189abyz/:?@_`{", 0, 0);
    test_func ("e_char_isxdigit", e_char_isxdigit, "0189afAF", "gzGZ/:?@_`{", 0, 0);
}

static void
test_func (const char *name,
           int (*func) (char),
           const char *accept,
           const char *reject,
           int accept_nul,
           int accept_nonascii)
{
    while (*accept) {
        e_test_assert (name, func (*accept));
        accept += 1;
    }
    while (*reject) {
        e_test_assert (name, !func (*reject));
        reject += 1;
    }
    e_test_assert (name, !(func (0) ^ accept_nul));
    e_test_assert (name, !(func (~0) ^ accept_nonascii));
}
