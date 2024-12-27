#ifndef _EMPOWER_RBUF_H_
#define _EMPOWER_RBUF_H_

/*! e_rbuf ********************************************************************
 * 
 * This module provides "type-generic" ringbuffers.
 *
 ******************************************************************************/

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define __e_rbuf_ensure_user_has_to_use_semicolon() size_t strlen (const char *s)

/**
 * Declare the struct and functions required for a ringbuffer of type \T. The
 * type of the ringbuffer will be equal to \type_name. All functions will start
 * with \prefix. This belongs into a header file and requires the functions to
 * be implemented somewhere with `E_RBUF_IMPL`. For instance, to create a
 * ringbuffer with items of type `int`, use something like the following
 * snippet:
 *
 *  | #include <e_rbuf.h>
 *  |
 *  | E_RBUF_DECL (int, E_Rbuf_Int, e_rbuf_int);
 *  | E_RBUF_IMPL (int, E_Rbuf_Int, e_rbuf_int);
 *  |
 *  | int *buf = e_alloc (int, 64);
 *  | E_Rbuf_Int rbuf = e_rbuf_int_init (buf, 64);
 *  | do_stuff ();
 *  | e_free (buf);
 *
 * Usually, you `push()` to the front and `pop()` from the back. Alternatively,
 * you can `push_back()` to the back and `pop_front()` from the front to do it
 * in the reverse direction.
 *
 * The following figure illustrates the used terms:
 *
 *            [BACK......FRONT]
 *    +---+---+---+---+---+---+---+---+
 *    | . | . | D | A | T | A | . | . |
 *    +---+---+---+---+---+---+---+---+
 *              ^tail           ^head
 */
#define E_RBUF_DECL(T, type_name, prefix)                                      \
                                                                               \
	/**                                                                    \
	 * The type of the ringbuffer.
	 */                                                                    \
	typedef struct {                                                       \
		T *ptr;                                                        \
		size_t head;                                                   \
		size_t tail;                                                   \
		size_t len;                                                    \
		size_t cap;                                                    \
	} type_name;                                                           \
                                                                               \
	type_name prefix##_init (T *buf, size_t cap);                          \
	size_t prefix##_len (const type_name *rbuf);                           \
	size_t prefix##_remaining_cap (const type_name *rbuf);                 \
	bool prefix##_is_empty (const type_name *rbuf);                        \
	bool prefix##_is_full (const type_name *rbuf);                         \
	void prefix##_reset (type_name *rbuf);                                 \
	bool prefix##_try_push (type_name *rbuf, T item);                      \
	bool prefix##_try_push_back (type_name *rbuf, T item);                 \
	void prefix##_push (type_name *rbuf, T item);                          \
	void prefix##_push_back (type_name *rbuf, T item);                     \
	bool prefix##_pop (type_name *rbuf, T *item_out);                      \
	bool prefix##_pop_front (type_name *rbuf, T *item_out);                \
                                                                               \
	__e_rbuf_ensure_user_has_to_use_semicolon ()

/* implementation *************************************************************/

/**
 * Implement the functions required for a ringbuffer of type \T with type
 * \type_name and function prefix \prefix. This goes into a `.c` file and
 * requires there to be a declaration somewhere in a header file with
 * `E_RBUF_DECL`.
 */
#define E_RBUF_IMPL(T, type_name, prefix)                                      \
                                                                               \
	/**                                                                    \
	 * Initialise the ringbuffer based on a pre-existing buffer \buf with  \
	 * capacity \cap. The user is required to allocate and free the buffer \
	 * if necessary.                                                       \
	 */                                                                    \
	type_name                                                              \
	prefix##_init (T *buf, size_t cap)                                     \
	{                                                                      \
		assert (buf != NULL && "buf for e_rbuf_init cannot be null");  \
                                                                               \
		return (type_name) {                                           \
			.ptr = buf,                                            \
			.head = 0,                                             \
			.tail = 0,                                             \
			.len = 0,                                              \
			.cap = cap,                                            \
		};                                                             \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Get the number of items in the ringbuffer \rbuf.                    \
	 */                                                                    \
	size_t                                                                 \
	prefix##_len (const type_name *rbuf)                                   \
	{                                                                      \
		return rbuf->len;                                              \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Get the remaining capacity of the ringbuffer \rbuf.                 \
	 */                                                                    \
	size_t                                                                 \
	prefix##_remaining_cap (const type_name *rbuf)                         \
	{                                                                      \
		return rbuf->cap - rbuf->len;                                  \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Check if the ringbuffer \rbuf is empty.                             \
	 */                                                                    \
	bool                                                                   \
	prefix##_is_empty (const type_name *rbuf)                              \
	{                                                                      \
		return rbuf->len == 0;                                         \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Check if the ringbuffer \rbuf is full.                              \
	 */                                                                    \
	bool                                                                   \
	prefix##_is_full (const type_name *rbuf)                               \
	{                                                                      \
		return rbuf->len == rbuf->cap;                                 \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Reset the ringbuffer \rbuf, i.e. "remove" all items.                \
	 */                                                                    \
	void                                                                   \
	prefix##_reset (type_name *rbuf)                                       \
	{                                                                      \
		if (!rbuf) return;                                             \
		rbuf->len = 0;                                                 \
		rbuf->head = 0;                                                \
		rbuf->tail = 0;                                                \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Add \item to the front of the ringbuffer \rbuf. If the ringbuffer   \
	 * is full, the tail of the ringbuffer will be overwritten.            \
	 */                                                                    \
	void                                                                   \
	prefix##_push (type_name *rbuf, T item)                                \
	{                                                                      \
		if (!rbuf) return;                                             \
                                                                               \
		if (rbuf->len == rbuf->cap) {                                  \
			rbuf->tail = (rbuf->tail + 1) % rbuf->cap;             \
		} else {                                                       \
			rbuf->len += 1;                                        \
		}                                                              \
                                                                               \
		rbuf->ptr[rbuf->head] = item;                                  \
		rbuf->head = (rbuf->head + 1) % rbuf->cap;                     \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Add \item to the back of the ringbuffer \rbuf. If the ringbuffer    \
	 * is full, the head of the ringbuffer will be overwritten.            \
	 */                                                                    \
	void                                                                   \
	prefix##_push_back (type_name *rbuf, T item)                           \
	{                                                                      \
		if (!rbuf) return;                                             \
                                                                               \
		if (rbuf->len == rbuf->cap) {                                  \
			rbuf->head = (rbuf->head + rbuf->cap - 1) % rbuf->cap; \
		} else {                                                       \
			rbuf->len += 1;                                        \
		}                                                              \
                                                                               \
		rbuf->tail = (rbuf->tail + rbuf->cap - 1) % rbuf->cap;         \
		rbuf->ptr[rbuf->tail] = item;                                  \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Try to add \item to the front of the ringbuffer \rbuf. If the       \
	 * ringbuffer is full, no action will be performed and `false` will be \
	 * returned. Otherwise, the item will be added and `true` is returned. \
	 */                                                                    \
	bool                                                                   \
	prefix##_try_push (type_name *rbuf, T item)                            \
	{                                                                      \
		if (!rbuf) return false;                                       \
		if (rbuf->len == rbuf->cap) return false;                      \
                                                                               \
		rbuf->len += 1;                                                \
		rbuf->ptr[rbuf->head] = item;                                  \
		rbuf->head = (rbuf->head + 1) % rbuf->cap;                     \
		return true;                                                   \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Try to add \item to the back of the ringbuffer \rbuf. If the        \
	 * ringbuffer is full, no action will be performed and `false` will be \
	 * returned. Otherwise, the item will be added and `true` is returned. \
	 */                                                                    \
	bool                                                                   \
	prefix##_try_push_back (type_name *rbuf, T item)                       \
	{                                                                      \
		if (!rbuf) return false;                                       \
		if (rbuf->len == rbuf->cap) return false;                      \
                                                                               \
		rbuf->len += 1;                                                \
		rbuf->tail = (rbuf->tail + rbuf->cap - 1) % rbuf->cap;         \
		rbuf->ptr[rbuf->tail] = item;                                  \
		return true;                                                   \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Try to pop an item from the back of the ringbuffer \rbuf. If the    \
	 * ringbuffer is not empty, the item will be removed and written to    \
	 * \item_out, and `true` will be returned. If the ringbuffer is empty, \
	 * no action will be performed, and `false` will be returned. If the   \
	 * \item_out parameter is `nullptr`, nothing will be written to it,    \
	 * but the item will still be popped and `true` or `false` will be     \
	 * returned.                                                           \
	 */                                                                    \
	bool                                                                   \
	prefix##_pop (type_name *rbuf, T *item_out)                            \
	{                                                                      \
		if (!rbuf) return false;                                       \
		if (rbuf->len == 0) return false;                              \
                                                                               \
		rbuf->len -= 1;                                                \
		rbuf->head = (rbuf->head + rbuf->cap - 1) % rbuf->cap;         \
		if (item_out) *item_out = rbuf->ptr[rbuf->head];               \
		return true;                                                   \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Try to pop an item from the front of the ringbuffer \rbuf. If the   \
	 * ringbuffer is not empty, the item will be removed and written to    \
	 * \item_out, and `true` will be returned. If the ringbuffer is empty, \
	 * no action will be performed, and `false` will be returned. If the   \
	 * \item_out parameter is `nullptr`, nothing will be written to it,    \
	 * but the item will still be popped and `true` or `false` will be     \
	 * returned.                                                           \
	 */                                                                    \
	bool                                                                   \
	prefix##_pop_front (type_name *rbuf, T *item_out)                      \
	{                                                                      \
		if (!rbuf) return false;                                       \
		if (rbuf->len == 0) return false;                              \
                                                                               \
		rbuf->len -= 1;                                                \
		if (item_out) *item_out = rbuf->ptr[rbuf->tail];               \
		rbuf->tail = (rbuf->tail + 1) % rbuf->cap;                     \
		return true;                                                   \
	}                                                                      \
                                                                               \
	__e_rbuf_ensure_user_has_to_use_semicolon ()

#endif /* _EMPOWER_VEC_H_ */
