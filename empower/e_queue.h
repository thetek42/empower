#ifndef E_QUEUE_H_
#define E_QUEUE_H_

/**************************************************************************************************
 *
 * Empower / e_queue.h - Public Domain - https://git.tjdev.de/thetek/empower
 *
 * This module implements dynamically allocated, resizable, double-ended queues for generic types.
 *
 * It can be used as follows:
 *
 * ```
 * E_Queue (int) int_queue = e_queue_init ();
 * e_queue_push (&int_queue, 1);
 * e_queue_push (&int_queue, 2);
 * int popped;
 * bool x = e_queue_pop (&int_queue, &popped); // true
 * bool y = e_queue_pop (&int_queue, &popped); // true
 * bool z = e_queue_pop (&int_queue, &popped); // false (no items left)
 * e_queue_deinit (&int_queue);
 * ```
 *
 * In the case where the queue definition has to be reused for multiple variables and those
 * variables have to have a compatible type, a `typedef` can be used:
 *
 * ```
 * typedef E_Queue (int) Int_Queue;
 * Int_Queue first_list;
 * Int_Queue second_list;
 * ```
 *
 * Usually, you `push()` to the front and `pop()` from the back. Alternatively, you can
 * `push_back()` to the back and `pop_front()` from the front to do it in the reverse direction.
 *
 * The following figure illustrates the used terms:
 *
 *            [BACK......FRONT]
 *    +---+---+---+---+---+---+---+---+
 *    | . | . | D | A | T | A | . | . |
 *    +---+---+---+---+---+---+---+---+
 *              ^tail           ^head
 *
 * On allocation failure, an error message is printed and the programme is aborted.
 *
 **************************************************************************************************/

#include <stddef.h>

/* compatibility annoyances: */
#ifndef E_TYPEOF
# if __STDC_VERSION__ >= 202311L
#  define E_TYPEOF(x) typeof (x)
# else
#  define E_TYPEOF(x) __typeof__ (x)
# endif
#endif /* E_TYPEOF */

/**
 * Generic resizable double-ended queue
 */
#define E_Queue(T)                                                                                 \
    union {                                                                                        \
        E_Queue_Data data;                                                                         \
        T *type; /* NOLINT */                                                                      \
    }

/**
 * Initialise a new queue.
 *
 * No memory is allocated yet.
 */
#define e_queue_init() {0}

/**
 * Free the memory occupied by the queue.
 */
#define e_queue_deinit(queue) e_queue__deinit (&(queue)->data)

/**
 * Obtain the length (i.e. the number of contained items) of the queue.
 */
#define e_queue_len(queue) (queue)->data.len

/**
 * Add an item to the front of the queue.
 */
#define e_queue_push(queue, item)                                                                  \
    do {                                                                                           \
        E_TYPEOF (*(queue)->type) e_queue__item = (item);                                          \
        e_queue__push (&(queue)->data, &e_queue__item, sizeof (*(queue)->type));                   \
    } while (0)

/**
 * Add an item to the front of the queue (but the item is passed as a pointer).
 */
#define e_queue_push_ref(queue, item_ptr)                                                          \
    e_queue__push (&(queue)->data, (1 ? (item_ptr) : (queue)->type), sizeof (*(queue)->type))

/**
 * Add an item to the back of the queue.
 */
#define e_queue_push_back(queue, item)                                                             \
    do {                                                                                           \
        E_TYPEOF (*(queue)->type) e_queue__item = (item);                                          \
        e_queue__push_back (&(queue)->data, &e_queue__item, sizeof (*(queue)->type));              \
    } while (0)

/**
 * Add an item to the back of the queue (but the item is passed as a pointer).
 */
#define e_queue_push_back_ref(queue, item_ptr)                                                     \
    e_queue__push_back (&(queue)->data, (1 ? (item_ptr) : (queue)->type), sizeof (*(queue)->type))

/**
 * Try to pop an item from the back of the queue `rbuf`.
 *
 * If the queue is not empty, the item will be removed and written to `out`, and a non-zero
 * (true) value will be returned. If the queue is empty, no action will be performed, and
 * zero (false) will be returned.
 *
 * If the `out` parameter is `NULL`, nothing will be written to it, but the item will still be
 * popped and `true` or `false` will be returned.
 */
#define e_queue_pop(queue, out)                                                                    \
    e_queue__pop (&(queue)->data, (1 ? (out) : (queue)->type), sizeof (*(queue)->type))

/**
 * Try to pop an item from the front of the queue `rbuf`.
 *
 * If the queue is not empty, the item will be removed and written to `out`, and a non-zero
 * (true) value will be returned. If the queue is empty, no action will be performed, and
 * zero (false) will be returned.
 *
 * If the `out` parameter is `NULL`, nothing will be written to it, but the item will still be
 * popped and `true` or `false` will be returned.
 */
#define e_queue_pop_front(queue, out)                                                              \
    e_queue__pop_front (&(queue)->data, (1 ? (out) : (queue)->type), sizeof (*(queue)->type))

typedef struct {
    void *ptr;
    size_t cap;
    size_t len;
    size_t head;
    size_t tail;
} E_Queue_Data;

void e_queue__deinit (E_Queue_Data *queue);
void e_queue__push (E_Queue_Data *queue, const void *item, size_t item_size);
void e_queue__push_back (E_Queue_Data *queue, const void *item, size_t item_size);
int e_queue__pop (E_Queue_Data *queue, void *out, size_t item_size);
int e_queue__pop_front (E_Queue_Data *queue, void *out, size_t item_size);

/**************************************************************************************************/

#ifdef E_QUEUE_IMPL

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define E_QUEUE__INIT_CAP 32

void e_queue__reserve (E_Queue_Data *queue, size_t cap, size_t item_size);

void
e_queue__deinit (E_Queue_Data *queue)
{
    free (queue->ptr);
}

void
e_queue__reserve (E_Queue_Data *queue, size_t cap, size_t item_size)
{
    unsigned char *ptr;
    size_t old_cap;

    /* check capacity */
    if (cap <= queue->cap) return;
    old_cap = queue->cap;
    if (queue->cap == 0) queue->cap = E_QUEUE__INIT_CAP;
    while (queue->cap < cap)
        queue->cap *= 2;

    /* reallocate */
    ptr = realloc (queue->ptr, queue->cap * item_size);
    if (ptr == NULL) {
        fprintf (stderr, "[e_queue] allocation failed!\n");
        abort ();
    }
    queue->ptr = ptr;

    /* move front if it would be cut off by allocation */
    if (queue->len > 0 && queue->head <= queue->tail) {
        memcpy (&ptr[item_size * old_cap], queue->ptr, item_size * queue->head);
        queue->head += old_cap;
    }
}

void
e_queue__push (E_Queue_Data *queue, const void *item, size_t item_size)
{
    unsigned char *slot;
    unsigned char *ptr;

    e_queue__reserve (queue, queue->len + 1, item_size);
    ptr = queue->ptr;
    slot = &ptr[queue->head * item_size];
    memcpy (slot, item, item_size);
    queue->len += 1;
    queue->head = (queue->head + 1) % queue->cap;
}

void
e_queue__push_back (E_Queue_Data *queue, const void *item, size_t item_size)
{
    unsigned char *slot;
    unsigned char *ptr;

    e_queue__reserve (queue, queue->len + 1, item_size);
    queue->tail = (queue->tail + queue->cap - 1) % queue->cap;
    ptr = queue->ptr;
    slot = &ptr[queue->tail * item_size];
    memcpy (slot, item, item_size);
    queue->len += 1;
}

int
e_queue__pop (E_Queue_Data *queue, void *out, size_t item_size)
{
    unsigned char *slot;
    unsigned char *ptr;

    if (queue->len == 0) return 0;

    queue->len -= 1;
    if (out != NULL) {
        ptr = queue->ptr;
        slot = &ptr[queue->tail * item_size];
        memcpy (out, slot, item_size);
    }
    queue->tail = (queue->tail + 1) % queue->cap;
    return 1;
}

int
e_queue__pop_front (E_Queue_Data *queue, void *out, size_t item_size)
{
    unsigned char *slot;
    unsigned char *ptr;

    if (queue->len == 0) return 0;

    queue->len -= 1;
    queue->head = (queue->head + queue->cap - 1) % queue->cap;
    if (out != NULL) {
        ptr = queue->ptr;
        slot = &ptr[queue->head * item_size];
        memcpy (out, slot, item_size);
    }
    return 1;
}

#endif /* E_QUEUE_IMPL */

#endif /* E_QUEUE_H_ */
