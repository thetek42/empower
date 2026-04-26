#define E_RBUF_IMPL
#include "e_macro.h"
#include "e_rbuf.h"
#include "e_test.h"

#include <stddef.h>

void
test_rbuf (void)
{
    static int buffer[4]; /* static for C89 compliance */
    int out = 0;

    /* e_rbuf_init */
    E_Rbuf (int) rbuf = e_rbuf_init (buffer, E_COUNTOF (buffer));
    e_test_assert_eq ("e_rbuf_init len", size_t, e_rbuf_len (&rbuf), 0);
    e_test_assert_eq ("e_rbuf_init cap", size_t, e_rbuf_cap (&rbuf), 4);
    e_test_assert ("e_rbuf_init is_empty", e_rbuf_is_empty (&rbuf));
    e_test_assert ("e_rbuf_init is_full", !e_rbuf_is_full (&rbuf));

    e_rbuf_push (&rbuf, 10);
    e_test_assert_eq ("e_rbuf_push len", size_t, e_rbuf_len (&rbuf), 1);
    e_test_assert ("e_rbuf_push is_empty", !e_rbuf_is_empty (&rbuf));
    e_test_assert ("e_rbuf_push is_full", !e_rbuf_is_full (&rbuf));
    e_test_assert_eq ("e_rbuf_push head", size_t, rbuf.data.head, 1);
    e_test_assert_eq ("e_rbuf_push tail", size_t, rbuf.data.tail, 0);
    e_test_assert_eq ("e_rbuf_push ptr[0]", int, ((int *) rbuf.data.ptr)[0], 10);

    e_rbuf_push_back (&rbuf, 20);
    e_test_assert_eq ("e_rbuf_push_back len", size_t, e_rbuf_len (&rbuf), 2);
    e_test_assert_eq ("e_rbuf_push_back head", size_t, rbuf.data.head, 1);
    e_test_assert_eq ("e_rbuf_push_back tail", size_t, rbuf.data.tail, 3);
    e_test_assert_eq ("e_rbuf_push ptr[3]", int, ((int *) rbuf.data.ptr)[3], 20);

    e_rbuf_push_back (&rbuf, 30);
    e_rbuf_push (&rbuf, 40);
    e_test_assert_eq ("e_rbuf_push full len", size_t, e_rbuf_len (&rbuf), 4);
    e_test_assert ("e_rbuf_push full is_empty", !e_rbuf_is_empty (&rbuf));
    e_test_assert ("e_rbuf_push full is_full", e_rbuf_is_full (&rbuf));
    e_test_assert_eq ("e_rbuf_push full head", size_t, rbuf.data.head, 2);
    e_test_assert_eq ("e_rbuf_push full tail", size_t, rbuf.data.tail, 2);
    e_test_assert_eq ("e_rbuf_push full ptr[0]", int, ((int *) rbuf.data.ptr)[0], 10);
    e_test_assert_eq ("e_rbuf_push full ptr[1]", int, ((int *) rbuf.data.ptr)[1], 40);
    e_test_assert_eq ("e_rbuf_push full ptr[2]", int, ((int *) rbuf.data.ptr)[2], 30);
    e_test_assert_eq ("e_rbuf_push full ptr[3]", int, ((int *) rbuf.data.ptr)[3], 20);

    e_rbuf_push (&rbuf, 50);
    e_test_assert_eq ("e_rbuf_push override len", size_t, e_rbuf_len (&rbuf), 4);
    e_test_assert_eq ("e_rbuf_push override head", size_t, rbuf.data.head, 3);
    e_test_assert_eq ("e_rbuf_push override tail", size_t, rbuf.data.tail, 3);
    e_test_assert_eq ("e_rbuf_push override ptr[0]", int, ((int *) rbuf.data.ptr)[0], 10);
    e_test_assert_eq ("e_rbuf_push override ptr[1]", int, ((int *) rbuf.data.ptr)[1], 40);
    e_test_assert_eq ("e_rbuf_push override ptr[2]", int, ((int *) rbuf.data.ptr)[2], 50);
    e_test_assert_eq ("e_rbuf_push override ptr[3]", int, ((int *) rbuf.data.ptr)[3], 20);

    e_rbuf_push_back (&rbuf, 60);
    e_test_assert_eq ("e_rbuf_push_back override len", size_t, e_rbuf_len (&rbuf), 4);
    e_test_assert_eq ("e_rbuf_push_back override head", size_t, rbuf.data.head, 2);
    e_test_assert_eq ("e_rbuf_push_back override tail", size_t, rbuf.data.tail, 2);
    e_test_assert_eq ("e_rbuf_push_back override ptr[0]", int, ((int *) rbuf.data.ptr)[0], 10);
    e_test_assert_eq ("e_rbuf_push_back override ptr[1]", int, ((int *) rbuf.data.ptr)[1], 40);
    e_test_assert_eq ("e_rbuf_push_back override ptr[2]", int, ((int *) rbuf.data.ptr)[2], 60);
    e_test_assert_eq ("e_rbuf_push_back override ptr[3]", int, ((int *) rbuf.data.ptr)[3], 20);

    e_test_assert ("e_rbuf_pop success", e_rbuf_pop (&rbuf, &out));
    e_test_assert_eq ("e_rbuf_pop success len", size_t, e_rbuf_len (&rbuf), 3);
    e_test_assert_eq ("e_rbuf_pop success head", size_t, rbuf.data.head, 2);
    e_test_assert_eq ("e_rbuf_pop success tail", size_t, rbuf.data.tail, 3);
    e_test_assert_eq ("e_rbuf_pop success out", int, out, 60);

    e_test_assert ("e_rbuf_pop_front success", e_rbuf_pop_front (&rbuf, &out));
    e_test_assert_eq ("e_rbuf_pop_front success len", size_t, e_rbuf_len (&rbuf), 2);
    e_test_assert_eq ("e_rbuf_pop_front success head", size_t, rbuf.data.head, 1);
    e_test_assert_eq ("e_rbuf_pop_front success tail", size_t, rbuf.data.tail, 3);
    e_test_assert_eq ("e_rbuf_pop_front success out", int, out, 40);

    e_rbuf_pop (&rbuf, NULL);
    e_rbuf_pop (&rbuf, NULL);

    e_test_assert ("e_rbuf_pop failure", !e_rbuf_pop (&rbuf, &out));
    e_test_assert ("e_rbuf_pop_front failure", !e_rbuf_pop_front (&rbuf, &out));
}
