#ifndef E_RBUF_H_
#define E_RBUF_H_

/**************************************************************************************************
 *
 * Empower / e_rbuf.h - Public Domain - https://git.tjdev.de/thetek/empower
 *
 * This module implements non-resizable, generic ringbuffers. No allocations are performed in the
 * library as the user has to provide the memory for the ringbuffer.
 *
 * It can be used as follows:
 *
 * ```
 * int *memory = malloc (sizeof (int) * 128);
 * E_Rbuf (int) int_rbuf = e_rbuf_init (memory, 128);
 * e_rbuf_push (&int_rbuf, 1);
 * e_rbuf_push (&int_rbuf, 2);
 * int popped;
 * bool x = e_rbuf_pop (&int_rbuf, &popped); // true
 * bool y = e_rbuf_pop (&int_rbuf, &popped); // true
 * bool z = e_rbuf_pop (&int_rbuf, &popped); // false (no items left)
 * e_rbuf_deinit (&int_rbuf);
 * ```
 *
 * In the case where the ringbuffer definition has to be reused for multiple variables and those
 * variables have to have a compatible type, a `typedef` can be used:
 *
 * ```
 * typedef E_Rbuf (int) Int_Rbuf;
 * Int_Rbuf first_rbuf;
 * Int_Rbuf second_rbuf;
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
 * Generic ringbuffer
 */
#define E_Rbuf(T)                                                                                  \
    union {                                                                                        \
        E_Rbuf_Data data;                                                                          \
        T *type; /* NOLINT */                                                                      \
    }

/**
 * Initialise a new ringbuffer.
 *
 * The arguments `T *ptr` and `size_t cap` refer to the memory that the ringbuffer will use.
 * `ptr` is the pointer, and `cap` is the capacity, i.e. the number of elements that have space.
 *
 * For instance, this can be used as follows:
 * ```
 * int *memory = malloc (sizeof (int) * 128);
 * E_Rbuf (int) rbuf = e_rbuf_init (memory, 128);
 * ```
 *
 * If this macro does not work for you (e.g. when you are using C89), you can manually initialize
 * the ringbuffer as follows:
 * ```
 * E_Rbuf (int) rbuf = {0};
 * rbuf.data.buffer = buffer;
 * rbuf.data.cap = cap;
 * ```
 */
#define e_rbuf_init(ptr, cap) {{(ptr), (cap), 0, 0, 0}}

/**
 * Obtain the length (i.e. the number of contained items) of the ringbuffer.
 */
#define e_rbuf_len(rbuf) (rbuf)->data.len

/**
 * Obtain the capacity (i.e. the maximum number of items that can be added) of the ringbuffer.
 */
#define e_rbuf_cap(rbuf) (rbuf)->data.cap

/**
 * Check if the ringbuffer is empty.
 */
#define e_rbuf_is_empty(rbuf) ((rbuf)->data.len == 0)

/**
 * Check if the ringbuffer is full.
 */
#define e_rbuf_is_full(rbuf) ((rbuf)->data.len == (rbuf)->data.cap)

/**
 * Add an item to the front of the ringbuffer. If the ringbuffer is full, the tail of the
 * ringbuffer will be overwritten.
 */
#define e_rbuf_push(rbuf, item)                                                                    \
    do {                                                                                           \
        E_TYPEOF (*(rbuf)->type) e_rbuf__item = (item);                                            \
        e_rbuf__push (&(rbuf)->data, &e_rbuf__item, sizeof (*(rbuf)->type));                       \
    } while (0)

/**
 * Add an item to the back of the ringbuffer. If the ringbuffer is full, the head of the
 * ringbuffer will be overwritten.
 */
#define e_rbuf_push_back(rbuf, item)                                                               \
    do {                                                                                           \
        E_TYPEOF (*(rbuf)->type) e_rbuf__item = (item);                                            \
        e_rbuf__push_back (&(rbuf)->data, &e_rbuf__item, sizeof (*(rbuf)->type));                  \
    } while (0)

/**
 * Try to pop an item from the back of the ringbuffer `rbuf`.
 *
 * If the ringbuffer is not empty, the item will be removed and written to `out`, and a non-zero
 * (true) value will be returned. If the ringbuffer is empty, no action will be performed, and
 * zero (false) will be returned.
 *
 * If the `out` parameter is `NULL`, nothing will be written to it, but the item will still be
 * popped and `true` or `false` will be returned.
 */
#define e_rbuf_pop(rbuf, out) e_rbuf__pop (&(rbuf)->data, (out), sizeof (*(rbuf)->type))

/**
 * Try to pop an item from the front of the ringbuffer `rbuf`.
 *
 * If the ringbuffer is not empty, the item will be removed and written to `out`, and a non-zero
 * (true) value will be returned. If the ringbuffer is empty, no action will be performed, and
 * zero (false) will be returned.
 *
 * If the `out` parameter is `NULL`, nothing will be written to it, but the item will still be
 * popped and `true` or `false` will be returned.
 */
#define e_rbuf_pop_front(rbuf, out) e_rbuf__pop_front (&(rbuf)->data, (out), sizeof (*(rbuf)->type))

typedef struct {
    void *ptr;
    size_t cap;
    size_t len;
    size_t head;
    size_t tail;
} E_Rbuf_Data;

void e_rbuf__push (E_Rbuf_Data *rbuf, const void *item, size_t item_size);
void e_rbuf__push_back (E_Rbuf_Data *rbuf, const void *item, size_t item_size);
int e_rbuf__pop (E_Rbuf_Data *rbuf, void *out, size_t item_size);
int e_rbuf__pop_front (E_Rbuf_Data *rbuf, void *out, size_t item_size);

/**************************************************************************************************/

#ifdef E_RBUF_IMPL

void
e_rbuf__push (E_Rbuf_Data *rbuf, const void *item, size_t item_size)
{
    const unsigned char *item_uchar;
    unsigned char *item_ptr;
    unsigned char *ptr;
    size_t i;

    if (rbuf->len == rbuf->cap) {
        /* overwrite tail */
        rbuf->tail = (rbuf->tail + 1) % rbuf->cap;
    } else {
        rbuf->len += 1;
    }

    /* copy item into buffer */
    ptr = rbuf->ptr;
    item_ptr = &ptr[item_size * rbuf->head];
    item_uchar = item;
    for (i = 0; i < item_size; i++) {
        item_ptr[i] = item_uchar[i];
    }

    /* advance head */
    rbuf->head = (rbuf->head + 1) % rbuf->cap;
}

void
e_rbuf__push_back (E_Rbuf_Data *rbuf, const void *item, size_t item_size)
{
    const unsigned char *item_uchar;
    unsigned char *item_ptr;
    unsigned char *ptr;
    size_t i;

    if (rbuf->len == rbuf->cap) {
        /* overwrite head */
        rbuf->head = (rbuf->head + rbuf->cap - 1) % rbuf->cap;
    } else {
        rbuf->len += 1;
    }

    /* advance tail */
    rbuf->tail = (rbuf->tail + rbuf->cap - 1) % rbuf->cap;

    /* copy item into buffer */
    ptr = rbuf->ptr;
    item_ptr = &ptr[item_size * rbuf->tail];
    item_uchar = item;
    for (i = 0; i < item_size; i++) {
        item_ptr[i] = item_uchar[i];
    }
}

int
e_rbuf__pop (E_Rbuf_Data *rbuf, void *out, size_t item_size)
{
    unsigned char *out_uchar;
    unsigned char *ptr_item;
    unsigned char *ptr;
    size_t i;

    if (rbuf->len == 0) return 0;

    rbuf->len -= 1;
    if (out != NULL) {
        ptr = rbuf->ptr;
        ptr_item = &ptr[item_size * rbuf->tail];
        out_uchar = out;
        for (i = 0; i < item_size; i++) {
            out_uchar[i] = ptr_item[i];
        }
    }
    rbuf->tail = (rbuf->tail + 1) % rbuf->cap;
    return 1;
}

int
e_rbuf__pop_front (E_Rbuf_Data *rbuf, void *out, size_t item_size)
{
    unsigned char *out_uchar;
    unsigned char *ptr_item;
    unsigned char *ptr;
    size_t i;

    if (rbuf->len == 0) return 0;

    rbuf->len -= 1;
    rbuf->head = (rbuf->head + rbuf->cap - 1) % rbuf->cap;
    if (out != NULL) {
        ptr = rbuf->ptr;
        ptr_item = &ptr[item_size * rbuf->head];
        out_uchar = out;
        for (i = 0; i < item_size; i++) {
            out_uchar[i] = ptr_item[i];
        }
    }
    return 1;
}

#endif /* E_RBUF_IMPL */

#endif /* E_RBUF_H_ */
