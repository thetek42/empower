#define E_QUEUE_IMPL
#include "e_queue.h"
#include "e_test.h"

#include <stdlib.h>

void
test_queue (void)
{
    E_Queue (int) queue = e_queue_init ();
    int out = 0;

    /* HACK: manually allocate just 2 elements so that we can test */
    queue.data.ptr = malloc (2 * sizeof (int));
    queue.data.cap = 2;

    e_queue_push (&queue, 10);
    e_queue_push (&queue, 20);
    e_test_assert_eq ("e_queue_push len", size_t, e_queue_len (&queue), 2);
    e_test_assert_eq ("e_queue_push tail", size_t, queue.data.tail, 0);
    e_test_assert_eq ("e_queue_push head", size_t, queue.data.head, 0);
    e_test_assert_eq ("e_queue_push ptr[0]", int, ((int *) queue.data.ptr)[0], 10);
    e_test_assert_eq ("e_queue_push ptr[1]", int, ((int *) queue.data.ptr)[1], 20);

    e_queue_push (&queue, 30);
    e_queue_push (&queue, 40);
    e_test_assert_eq ("e_queue_push easy resize len", size_t, e_queue_len (&queue), 4);
    e_test_assert_eq ("e_queue_push easy resize cap", size_t, queue.data.cap, 4);
    e_test_assert_eq ("e_queue_push ptr[0]", int, ((int *) queue.data.ptr)[0], 10);
    e_test_assert_eq ("e_queue_push ptr[2]", int, ((int *) queue.data.ptr)[2], 30); /* NOLINT */
    e_test_assert_eq ("e_queue_push ptr[3]", int, ((int *) queue.data.ptr)[3], 40); /* NOLINT */

    e_test_assert ("e_queue_pop 1", e_queue_pop (&queue, &out));
    e_test_assert_eq ("e_queue_pop 1 out", int, out, 10);
    e_test_assert_eq ("e_queue_pop 1 len", size_t, e_queue_len (&queue), 3);
    e_test_assert_eq ("e_queue_pop 1 tail", size_t, queue.data.tail, 1);
    e_test_assert ("e_queue_pop 2", e_queue_pop (&queue, &out));
    e_test_assert_eq ("e_queue_pop 2 out", int, out, 20);
    e_test_assert_eq ("e_queue_pop 2 len", size_t, e_queue_len (&queue), 2);
    e_test_assert_eq ("e_queue_pop 2 tail", size_t, queue.data.tail, 2);

    e_queue_push (&queue, 50);
    e_queue_push (&queue, 60);
    e_test_assert_eq ("e_queue_push resize cap before", size_t, queue.data.cap, 4);
    e_test_assert_eq ("e_queue_push resize head before", size_t, queue.data.head, 2);
    e_test_assert_eq ("e_queue_push resize tail before", size_t, queue.data.tail, 2);
    e_test_assert_eq ("e_queue_push resize ptr[2] before", int, ((int *) queue.data.ptr)[2], 30);
    e_test_assert_eq ("e_queue_push resize ptr[3] before", int, ((int *) queue.data.ptr)[3], 40);
    e_test_assert_eq ("e_queue_push resize ptr[0] before", int, ((int *) queue.data.ptr)[0], 50);
    e_test_assert_eq ("e_queue_push resize ptr[1] before", int, ((int *) queue.data.ptr)[1], 60);
    e_queue_push (&queue, 70);
    e_test_assert_eq ("e_queue_push resize cap", size_t, queue.data.cap, 8);
    e_test_assert_eq ("e_queue_push resize head", size_t, queue.data.head, 7);
    e_test_assert_eq ("e_queue_push resize tail", size_t, queue.data.tail, 2);
    e_test_assert_eq ("e_queue_push resize ptr[2] before", int, ((int *) queue.data.ptr)[2], 30);
    e_test_assert_eq ("e_queue_push resize ptr[3] before", int, ((int *) queue.data.ptr)[3], 40);
    e_test_assert_eq ("e_queue_push resize ptr[4] before", int, ((int *) queue.data.ptr)[4], 50);
    e_test_assert_eq ("e_queue_push resize ptr[5] before", int, ((int *) queue.data.ptr)[5], 60);
    e_test_assert_eq ("e_queue_push resize ptr[6] before", int, ((int *) queue.data.ptr)[6], 70);

    e_queue_push_back (&queue, 80);
    e_test_assert_eq ("e_queue_push_back tail", size_t, queue.data.tail, 1);
    e_test_assert_eq ("e_queue_push_back ptr[1] before", int, ((int *) queue.data.ptr)[1], 80);

    e_test_assert ("e_queue_pop_front 1", e_queue_pop_front (&queue, &out));
    e_test_assert_eq ("e_queue_pop_front 1 out", int, out, 70);
    e_test_assert_eq ("e_queue_pop_front 1 head", size_t, queue.data.head, 6);

    e_queue_deinit (&queue);
}
