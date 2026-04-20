#define E_SV_IMPL
#include "e_sv.h"
#include "e_test.h"

#include <stddef.h>
#include <stdio.h>

void
test_sv (void)
{
    E_Sv sv, sv2;
    char buf[20];

    /* e_sv_from_cstr, e_sv_from_cstr */
    sv = e_sv_from_cstr ("");
    e_test_assert_eq ("e_sv_from_cstr empty", size_t, sv.len, 0);
    sv = e_sv_from_cstr ("foo bar baz");
    e_test_assert_eq ("e_sv_from_cstr len", size_t, sv.len, 11);
    sv = e_sv_from_cstr ("");
    e_test_assert_eq ("e_sv_from_cstr empty", size_t, sv.len, 0);
    sv = e_sv_from_cstr ("foo bar baz");
    e_test_assert_eq ("e_sv_from_cstr", size_t, sv.len, 11);

    /* e_sv_from_cstr_FMT, e_sv_from_cstr_ARG */
    sv = e_sv_from_cstr ("foo bar baz");
    sprintf (buf, "x%" E_SV_FMT "y", E_SV_ARG (sv));
    e_test_assert_str_eq ("e_sv_from_cstr_FMT", buf, "xfoo bar bazy");

    /* e_sv_eq */
    sv = e_sv_from_cstr ("foo bar baz");
    e_test_assert ("e_sv_eq true", e_sv_eq (sv, e_sv_from_cstr ("foo bar baz")));
    e_test_assert ("e_sv_eq false", !e_sv_eq (sv, e_sv_from_cstr ("foo")));
    e_test_assert ("e_sv_eq empty", !e_sv_eq (sv, e_sv_from_cstr ("")));

    /* e_sv_starts_with */
    sv = e_sv_from_cstr ("foo bar baz");
    e_test_assert ("e_sv_starts_with true", e_sv_starts_with (sv, e_sv_from_cstr ("foo")));
    e_test_assert ("e_sv_starts_with false", !e_sv_starts_with (sv, e_sv_from_cstr ("bar")));
    e_test_assert ("e_sv_starts_with empty", e_sv_starts_with (sv, e_sv_from_cstr ("")));
    e_test_assert ("e_sv_starts_with full", e_sv_starts_with (sv, e_sv_from_cstr ("foo bar baz")));
    e_test_assert ("e_sv_starts_with too long",
                   !e_sv_starts_with (sv, e_sv_from_cstr ("foo bar baz qux")));

    /* e_sv_ends_with */
    sv = e_sv_from_cstr ("foo bar baz");
    e_test_assert ("e_sv_ends_with true", e_sv_ends_with (sv, e_sv_from_cstr ("baz")));
    e_test_assert ("e_sv_ends_with false", !e_sv_ends_with (sv, e_sv_from_cstr ("bar")));
    e_test_assert ("e_sv_ends_with empty", e_sv_ends_with (sv, e_sv_from_cstr ("")));
    e_test_assert ("e_sv_ends_with full", e_sv_ends_with (sv, e_sv_from_cstr ("foo bar baz")));
    e_test_assert ("e_sv_ends_with too long",
                   !e_sv_ends_with (sv, e_sv_from_cstr ("qux foo bar baz")));

    /* e_sv_chop_left */
    sv = e_sv_from_cstr ("foo bar baz");
    sv2 = e_sv_chop_left (&sv, 4);
    e_test_assert ("e_sv_chop_left 1 rem", e_sv_eq (sv, e_sv_from_cstr ("bar baz")));
    e_test_assert ("e_sv_chop_left 1 chop", e_sv_eq (sv2, e_sv_from_cstr ("foo ")));
    sv2 = e_sv_chop_left (&sv, 6);
    e_test_assert ("e_sv_chop_left 2 rem", e_sv_eq (sv, e_sv_from_cstr ("z")));
    e_test_assert ("e_sv_chop_left 2 chop", e_sv_eq (sv2, e_sv_from_cstr ("bar ba")));
    sv2 = e_sv_chop_left (&sv, 0);
    e_test_assert ("e_sv_chop_left 3 rem", e_sv_eq (sv, e_sv_from_cstr ("z")));
    e_test_assert ("e_sv_chop_left 3 chop", e_sv_eq (sv2, e_sv_from_cstr ("")));
    sv2 = e_sv_chop_left (&sv, 1);
    e_test_assert ("e_sv_chop_left 4 rem", e_sv_eq (sv, e_sv_from_cstr ("")));
    e_test_assert ("e_sv_chop_left 4 chop", e_sv_eq (sv2, e_sv_from_cstr ("z")));
    sv2 = e_sv_chop_left (&sv, 42);
    e_test_assert ("e_sv_chop_left 5 rem", e_sv_eq (sv, e_sv_from_cstr ("")));
    e_test_assert ("e_sv_chop_left 5 chop", e_sv_eq (sv2, e_sv_from_cstr ("")));
    sv = e_sv_from_cstr ("foo bar baz");
    sv2 = e_sv_chop_left (&sv, 42);
    e_test_assert ("e_sv_chop_left 6 rem", e_sv_eq (sv, e_sv_from_cstr ("")));
    e_test_assert ("e_sv_chop_left 6 chop", e_sv_eq (sv2, e_sv_from_cstr ("foo bar baz")));

    /* e_sv_chop_right */
    sv = e_sv_from_cstr ("foo bar baz");
    sv2 = e_sv_chop_right (&sv, 4);
    e_test_assert ("e_sv_chop_right 1 rem", e_sv_eq (sv, e_sv_from_cstr ("foo bar")));
    e_test_assert ("e_sv_chop_right 1 chop", e_sv_eq (sv2, e_sv_from_cstr (" baz")));
    sv2 = e_sv_chop_right (&sv, 6);
    e_test_assert ("e_sv_chop_right 2 rem", e_sv_eq (sv, e_sv_from_cstr ("f")));
    e_test_assert ("e_sv_chop_right 2 chop", e_sv_eq (sv2, e_sv_from_cstr ("oo bar")));
    sv2 = e_sv_chop_right (&sv, 0);
    e_test_assert ("e_sv_chop_right 3 rem", e_sv_eq (sv, e_sv_from_cstr ("f")));
    e_test_assert ("e_sv_chop_right 3 chop", e_sv_eq (sv2, e_sv_from_cstr ("")));
    sv2 = e_sv_chop_right (&sv, 1);
    e_test_assert ("e_sv_chop_right 4 rem", e_sv_eq (sv, e_sv_from_cstr ("")));
    e_test_assert ("e_sv_chop_right 4 chop", e_sv_eq (sv2, e_sv_from_cstr ("f")));
    sv2 = e_sv_chop_right (&sv, 42);
    e_test_assert ("e_sv_chop_right 5 rem", e_sv_eq (sv, e_sv_from_cstr ("")));
    e_test_assert ("e_sv_chop_right 5 chop", e_sv_eq (sv2, e_sv_from_cstr ("")));
    sv = e_sv_from_cstr ("foo bar baz");
    sv2 = e_sv_chop_right (&sv, 42);
    e_test_assert ("e_sv_chop_right 6 rem", e_sv_eq (sv, e_sv_from_cstr ("")));
    e_test_assert ("e_sv_chop_right 6 chop", e_sv_eq (sv2, e_sv_from_cstr ("foo bar baz")));
}
