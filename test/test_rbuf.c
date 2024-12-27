#include <e_rbuf.h>

#include <e_test.h>
#include <e_types.h>

E_RBUF_DECL (int, E_Rbuf_Int, e_rbuf_int);
E_RBUF_IMPL (int, E_Rbuf_Int, e_rbuf_int);

void
test_rbuf (void)
{
	E_Rbuf_Int rbuf;
	int buf[4];
	int out;

	rbuf = e_rbuf_int_init (buf, 4);
	e_test_assert_eq ("e_rbuf_init len", usize, e_rbuf_int_len (&rbuf), 0);
	e_test_assert_eq ("e_rbuf_init remaining_cap", usize, e_rbuf_int_remaining_cap (&rbuf), 4);
	e_test_assert ("e_rbuf_init empty is_empty", e_rbuf_int_is_empty (&rbuf));
	e_test_assert ("e_rbuf_init empty is_full", !e_rbuf_int_is_full (&rbuf));

	e_rbuf_int_push (&rbuf, 10);
	e_test_assert_eq ("e_rbuf_push len", usize, e_rbuf_int_len (&rbuf), 1);
	e_test_assert_eq ("e_rbuf_push remaining_cap", usize, e_rbuf_int_remaining_cap (&rbuf), 3);
	e_test_assert ("e_rbuf_push is_empty", !e_rbuf_int_is_empty (&rbuf));
	e_test_assert ("e_rbuf_push is_full", !e_rbuf_int_is_full (&rbuf));
	e_test_assert_eq ("e_rbuf_push head", usize, rbuf.head, 1);
	e_test_assert_eq ("e_rbuf_push tail", usize, rbuf.tail, 0);
	e_test_assert_eq ("e_rbuf_push ptr[0]", int, rbuf.ptr[0], 10);

	e_rbuf_int_push_back (&rbuf, 20);
	e_test_assert_eq ("e_rbuf_push_back len", usize, e_rbuf_int_len (&rbuf), 2);
	e_test_assert_eq ("e_rbuf_push_back remaining_cap", usize, e_rbuf_int_remaining_cap (&rbuf), 2);
	e_test_assert_eq ("e_rbuf_push_back head", usize, rbuf.head, 1);
	e_test_assert_eq ("e_rbuf_push_back tail", usize, rbuf.tail, 3);
	e_test_assert_eq ("e_rbuf_push_back ptr[3]", int, rbuf.ptr[3], 20);

	e_rbuf_int_push_back (&rbuf, 30);
	e_rbuf_int_push (&rbuf, 40);
	e_test_assert_eq ("e_rbuf_push full len", usize, e_rbuf_int_len (&rbuf), 4);
	e_test_assert_eq ("e_rbuf_push full remaining_cap", usize, e_rbuf_int_remaining_cap (&rbuf), 0);
	e_test_assert ("e_rbuf_push full is_empty", !e_rbuf_int_is_empty (&rbuf));
	e_test_assert ("e_rbuf_push full is_full", e_rbuf_int_is_full (&rbuf));
	e_test_assert_eq ("e_rbuf_push full head", usize, rbuf.head, 2);
	e_test_assert_eq ("e_rbuf_push full tail", usize, rbuf.tail, 2);
	e_test_assert_eq ("e_rbuf_push full ptr[0]", int, rbuf.ptr[0], 10);
	e_test_assert_eq ("e_rbuf_push full ptr[1]", int, rbuf.ptr[1], 40);
	e_test_assert_eq ("e_rbuf_push full ptr[2]", int, rbuf.ptr[2], 30);
	e_test_assert_eq ("e_rbuf_push full ptr[3]", int, rbuf.ptr[3], 20);

	e_rbuf_int_push (&rbuf, 50);
	e_test_assert_eq ("e_rbuf_push override len", usize, e_rbuf_int_len (&rbuf), 4);
	e_test_assert_eq ("e_rbuf_push override head", usize, rbuf.head, 3);
	e_test_assert_eq ("e_rbuf_push override tail", usize, rbuf.tail, 3);
	e_test_assert_eq ("e_rbuf_push override ptr[0]", int, rbuf.ptr[0], 10);
	e_test_assert_eq ("e_rbuf_push override ptr[1]", int, rbuf.ptr[1], 40);
	e_test_assert_eq ("e_rbuf_push override ptr[2]", int, rbuf.ptr[2], 50);
	e_test_assert_eq ("e_rbuf_push override ptr[3]", int, rbuf.ptr[3], 20);

	e_rbuf_int_push_back (&rbuf, 60);
	e_test_assert_eq ("e_rbuf_push_back override len", usize, e_rbuf_int_len (&rbuf), 4);
	e_test_assert_eq ("e_rbuf_push_back override head", usize, rbuf.head, 2);
	e_test_assert_eq ("e_rbuf_push_back override tail", usize, rbuf.tail, 2);
	e_test_assert_eq ("e_rbuf_push_back override ptr[0]", int, rbuf.ptr[0], 10);
	e_test_assert_eq ("e_rbuf_push_back override ptr[1]", int, rbuf.ptr[1], 40);
	e_test_assert_eq ("e_rbuf_push_back override ptr[2]", int, rbuf.ptr[2], 60);
	e_test_assert_eq ("e_rbuf_push_back override ptr[3]", int, rbuf.ptr[3], 20);

	e_test_assert ("e_rbuf_try_push failure ret", !e_rbuf_int_try_push (&rbuf, 70));
	e_test_assert_eq ("e_rbuf_try_push failure head", usize, rbuf.head, 2);
	e_test_assert_eq ("e_rbuf_try_push failure tail", usize, rbuf.tail, 2);

	e_test_assert ("e_rbuf_try_push_back failure ret", !e_rbuf_int_try_push_back (&rbuf, 80));
	e_test_assert_eq ("e_rbuf_try_push_back failure head", usize, rbuf.head, 2);
	e_test_assert_eq ("e_rbuf_try_push_back failure tail", usize, rbuf.tail, 2);

	e_test_assert ("e_rbuf_pop success ret", e_rbuf_int_pop (&rbuf, &out));
	e_test_assert_eq ("e_rbuf_pop success len", usize, e_rbuf_int_len (&rbuf), 3);
	e_test_assert_eq ("e_rbuf_pop success remaining_cap", usize, e_rbuf_int_remaining_cap (&rbuf), 1);
	e_test_assert_eq ("e_rbuf_pop success head", usize, rbuf.head, 1);
	e_test_assert_eq ("e_rbuf_pop success tail", usize, rbuf.tail, 2);
	e_test_assert_eq ("e_rbuf_pop success out", int, out, 40);

	e_test_assert ("e_rbuf_pop_front success ret", e_rbuf_int_pop_front (&rbuf, &out));
	e_test_assert_eq ("e_rbuf_pop_front success len", usize, e_rbuf_int_len (&rbuf), 2);
	e_test_assert_eq ("e_rbuf_pop_front success remaining_cap", usize, e_rbuf_int_remaining_cap (&rbuf), 2);
	e_test_assert_eq ("e_rbuf_pop_front success head", usize, rbuf.head, 1);
	e_test_assert_eq ("e_rbuf_pop_front success tail", usize, rbuf.tail, 3);
	e_test_assert_eq ("e_rbuf_pop_front success out", int, out, 60);

	e_rbuf_int_reset (&rbuf);
	e_test_assert_eq ("e_rbuf_reset len", usize, e_rbuf_int_len (&rbuf), 0);
	e_test_assert_eq ("e_rbuf_reset remaining_cap", usize, e_rbuf_int_remaining_cap (&rbuf), 4);
	e_test_assert ("e_rbuf_reset is_empty", e_rbuf_int_is_empty (&rbuf));
	e_test_assert ("e_rbuf_reset is_full", !e_rbuf_int_is_full (&rbuf));
	e_test_assert_eq ("e_rbuf_reset head", usize, rbuf.head, 0);
	e_test_assert_eq ("e_rbuf_reset tail", usize, rbuf.tail, 0);

	e_test_assert ("e_rbuf_pop failure", !e_rbuf_int_pop (&rbuf, &out));
	e_test_assert ("e_rbuf_pop_front failure", !e_rbuf_int_pop_front (&rbuf, &out));

	e_test_assert ("e_rbuf_try_push success ret", e_rbuf_int_try_push (&rbuf, 90));
	e_test_assert_eq ("e_rbuf_try_push success head", usize, rbuf.head, 1);
	e_test_assert_eq ("e_rbuf_try_push success tail", usize, rbuf.tail, 0);
	e_test_assert_eq ("e_rbuf_try_push success ptr[0]", int, rbuf.ptr[0], 90);

	e_test_assert ("e_rbuf_try_push_back success ret", e_rbuf_int_try_push_back (&rbuf, 100));
	e_test_assert_eq ("e_rbuf_try_push_back success head", usize, rbuf.head, 1);
	e_test_assert_eq ("e_rbuf_try_push_back success tail", usize, rbuf.tail, 3);
	e_test_assert_eq ("e_rbuf_try_push_back success ptr[3]", int, rbuf.ptr[3], 100);
}
