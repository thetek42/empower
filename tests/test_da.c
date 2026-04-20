#define E_DA_IMPL
#include "e_da.h"
#include "e_macro.h"
#include "e_test.h"

#include <stddef.h>

void
test_da (void)
{
    int vals[] = {4, 5, 6};
    int *ptr;

    /* e_da_init */
    E_Da (int) da = e_da_init ();
    e_test_assert_eq ("e_da_init", size_t, e_da_len (&da), 0);

    /* e_da_push */
    e_da_push (&da, 1);
    e_da_push (&da, 2);
    e_da_push (&da, 3);
    e_test_assert_eq ("e_da_push len", size_t, e_da_len (&da), 3);
    ptr = e_da_first (&da);
    e_test_assert_eq ("e_da_push ptr[0]", int, ptr[0], 1);
    e_test_assert_eq ("e_da_push ptr[1]", int, ptr[1], 2);
    e_test_assert_eq ("e_da_push ptr[2]", int, ptr[2], 3);

    /* e_da_nth */
    e_test_assert_eq ("e_da_nth 0", int, *e_da_nth (&da, 0), 1);
    e_test_assert_eq ("e_da_nth 1", int, *e_da_nth (&da, 1), 2);
    e_test_assert_eq ("e_da_nth 2", int, *e_da_nth (&da, 2), 3);

    /* e_da_extend */
    e_da_extend (&da, vals, E_COUNTOF (vals));
    e_test_assert_eq ("e_da_extend len", size_t, e_da_len (&da), 6);
    e_test_assert_eq ("e_da_extend ptr[0]", int, *e_da_nth (&da, 3), 4);
    e_test_assert_eq ("e_da_extend ptr[1]", int, *e_da_nth (&da, 4), 5);
    e_test_assert_eq ("e_da_extend ptr[2]", int, *e_da_nth (&da, 5), 6);

    /* e_da_pop */
    e_da_pop (&da, 2);
    e_test_assert_eq ("e_da_pop len", size_t, e_da_len (&da), 4);
    e_da_push (&da, 5);
    e_test_assert_eq ("e_da_pop push", size_t, e_da_len (&da), 5);
    e_test_assert_eq ("e_da_pop ptr[3]", int, *e_da_nth (&da, 3), 4);
    e_test_assert_eq ("e_da_pop ptr[4]", int, *e_da_nth (&da, 4), 5);

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    /* e_da_foreach */
    int sum = 0;
    e_da_foreach (&da, it) {
        sum += *it;
    }
    e_test_assert_eq ("e_da_foreach", int, sum, 15);
#endif

    e_da_deinit (&da);
}
