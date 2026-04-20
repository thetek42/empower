#define E_CONFIG_SB_SV_COMPAT
#define E_SB_IMPL
#include "e_sb.h"
#include "e_sv.h"
#include "e_test.h"

#include <stddef.h>

void
test_sb (void)
{
    E_Sb sb;
    char *buf = " bar";
    size_t prev_len;

    /* e_sb_init */
    sb = e_sb_init ();
    e_test_assert_eq ("e_sb_init len", size_t, sb.len, 0);
    e_test_assert ("e_sb_init sv", e_sv_eq (e_sb_to_sv (&sb), e_sv_from_cstr ("")));

    /* e_sb_append_char */
    e_sb_append_char (&sb, 'f');
    e_sb_append_char (&sb, 'o');
    e_sb_append_char (&sb, 'o');
    e_test_assert ("e_sb_append_char", e_sv_eq (e_sb_to_sv (&sb), e_sv_from_cstr ("foo")));

    /* e_sb_append_buf */
    e_sb_append_buf (&sb, buf, 3);
    e_test_assert ("e_sb_append_buf", e_sv_eq (e_sb_to_sv (&sb), e_sv_from_cstr ("foo ba")));

    /* e_sb_append */
    e_sb_append (&sb, buf);
    e_test_assert ("e_sb_append", e_sv_eq (e_sb_to_sv (&sb), e_sv_from_cstr ("foo ba bar")));

    /* e_sb_append_sv */
    e_sb_append_sv (&sb, e_sv_from_cstr (" baz"));
    e_test_assert ("e_sb_append_sv", e_sv_eq (e_sb_to_sv (&sb), e_sv_from_cstr ("foo ba bar baz")));

    /* e_sb_append_null */
    prev_len = sb.len;
    e_sb_append_null (&sb);
    e_test_assert_eq ("e_sb_append_null len", size_t, sb.len, prev_len);
    e_test_assert_eq ("e_sb_append_null sentinel", char, sb.ptr[sb.len], '\0');
    e_test_assert_str_eq ("e_sb_append_null str", sb.ptr, "foo ba bar baz");
    e_sb_append (&sb, " qux");
    e_test_assert_eq ("e_sb_append_null append len", size_t, sb.len, prev_len + 4);
    e_test_assert ("e_sb_append_null append str",
                   e_sv_eq (e_sb_to_sv (&sb), e_sv_from_cstr ("foo ba bar baz qux")));

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    /* e_sb_append_fmt */
    e_sb_append_fmt (&sb, " x%2.2fy", 13.3742);
    e_test_assert ("e_sb_append_fmt",
                   e_sv_eq (e_sb_to_sv (&sb), e_sv_from_cstr ("foo ba bar baz qux x13.37y")));
#endif

    e_sb_deinit (&sb);
}
