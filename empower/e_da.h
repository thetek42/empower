#ifndef E_DA_H_
#define E_DA_H_

/**************************************************************************************************
 *
 * Empower / e_da.h - Public Domain - https://git.tjdev.de/thetek/empower
 *
 * This module implements dynamically allocated, resizable arrays for generic types.
 *
 * It can be used as follows:
 *
 * ```
 * E_Da (int) int_list = e_da_init ();
 * e_da_push (&int_list, 1);
 * e_da_push (&int_list, 2);
 * e_da_push (&int_list, 3);
 * e_da_deinit (&int_list);
 * ```
 *
 * In the case where the dynamic array definition has to be reused for multiple
 * variables and those variables have to have a compatible type, a `typedef`
 * can be used:
 *
 * ```
 * typedef E_Da (int) Int_List;
 * Int_List first_list;
 * Int_List second_list;
 * ```
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
 * Generic dynamic array
 */
#define E_Da(T)                                                                                    \
    union {                                                                                        \
        E_Da_Data data;                                                                            \
        T *type; /* NOLINT */                                                                      \
    }

/**
 * Initialise a new dynamic array.
 *
 * No memory is allocated yet.
 */
#define e_da_init() {0}

/**
 * Free the memory occupied by the dynamic array.
 */
#define e_da_deinit(da) e_da__deinit (&(da)->data)

/**
 * Obtain the length (i.e. the number of contained items) of the dynamic array.
 */
#define e_da_len(da) (da)->data.len

/**
 * Obtain a pointer to the first item of the dynamic array.
 *
 * This does not perform any bounds checks. If the dynamic array is empty, NULL
 * may be returned (but this is not a guarantee).
 */
#define e_da_first(da) ((E_TYPEOF ((da)->type)) (da)->data.ptr)

/**
 * Obtain a pointer to the last item of the dynamic array.
 *
 * This does not perform any bounds checks.
 */
#define e_da_last(da) (&(e_da_first (da))[(da)->data.len - 1])

/**
 * Obtain a pointer to the nth item of the dynamic array.
 *
 * This does not perform any bounds checks.
 */
#define e_da_nth(da, n) (&(e_da_first (da))[(n)])

/**
 * Append a single item to the end of a dynamic array.
 */
#define e_da_push(da, item)                                                                        \
    do {                                                                                           \
        E_TYPEOF (*(da)->type) e_da__item = (item);                                                \
        e_da_extend ((da), &e_da__item, 1);                                                        \
    } while (0)

/**
 * Make room for an additional item in the dynamic array, but don’t initialize that item. The
 * pointer to the new item is returned, so that you can fill it as you please. The length of
 * the dynamic array is adjusted, so not filling in the new memory will cause UB.
 */
#define e_da_push_uninit(da)                                                                       \
    ((E_TYPEOF ((da)->type)) e_da__extend_uninit (&(da)->data, 1, sizeof (*(da)->type)))

/**
 * Extend the dynamic array by multiple items.
 */
#define e_da_extend(da, items, count)                                                              \
    e_da__extend (&(da)->data, (1 ? (items) : (da)->type), (count), sizeof (*(da)->type))

/**
 * Make room for `count` additional items in the dynamic array, but don’t initialize that item. The
 * pointer to the first new item is returned, so that you can fill it as you please. The length of
 * the dynamic array is adjusted, so not filling in the new memory will cause UB.
 */
#define e_da_extend_uninit(da, count)                                                              \
    ((E_TYPEOF ((da)->type)) e_da__extend_uninit (&(da)->data, (count), sizeof (*(da)->type)))

/**
 * Remove a certain number of items from the end of the dynamic array.
 */
#define e_da_pop(da, count) e_da__pop (&(da)->data, (count))

/**
 * Iterate over the dynamic array.
 *
 * This macro can be used as follows:
 *
 * ```
 * E_Da (int) int_list = e_da_init ();
 * // ...
 * e_da_foreach (&int_list, it) {
 *     printf ("%d\n", *it);
 * }
 * ```
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
# define e_da_foreach(da, it)                                                                      \
     for (E_TYPEOF ((da)->type) (it) = (da)->data.ptr;                                             \
          (it) < ((E_TYPEOF ((da)->type)) ((da)->data.ptr) + (da)->data.len); (it) += 1)
#endif

typedef struct {
    void *ptr;
    size_t len;
    size_t cap;
} E_Da_Data;

void e_da__deinit (E_Da_Data *da);
void e_da__reserve (E_Da_Data *da, size_t cap, size_t item_size);
void e_da__extend (E_Da_Data *da, void *data, size_t count, size_t item_size);
void *e_da__extend_uninit (E_Da_Data *da, size_t count, size_t item_size);
void e_da__pop (E_Da_Data *da, size_t count);

/**************************************************************************************************/

#ifdef E_DA_IMPL

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define E_DA__INIT_CAP 32

void
e_da__deinit (E_Da_Data *da)
{
    free (da->ptr);
}

void
e_da__reserve (E_Da_Data *da, size_t cap, size_t item_size)
{
    void *ptr;
    if (cap <= da->cap) return;
    if (da->cap == 0) da->cap = E_DA__INIT_CAP;
    while (da->cap < cap)
        da->cap *= 2;
    ptr = realloc (da->ptr, da->cap * item_size);
    if (ptr == NULL) {
        fprintf (stderr, "[e_da] allocation failed!\n");
        abort ();
    }
    da->ptr = ptr;
}

void
e_da__extend (E_Da_Data *da, void *data, size_t count, size_t item_size)
{
    unsigned char *ptr;
    e_da__reserve (da, da->len + count, item_size);
    ptr = da->ptr;
    memcpy (&ptr[da->len * item_size], data, count * item_size);
    da->len += count;
}

void *
e_da__extend_uninit (E_Da_Data *da, size_t count, size_t item_size)
{
    unsigned char *ptr;
    e_da__reserve (da, da->len + count, item_size);
    ptr = da->ptr;
    ptr = &ptr[item_size * da->len];
    da->len += count;
    return ptr;
}

void
e_da__pop (E_Da_Data *da, size_t count)
{
    if (count > da->len) count = da->len;
    da->len -= count;
}

#endif /* E_DA_IMPL */

#endif /* E_DA_H_ */
