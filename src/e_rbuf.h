/*! e_rbuf ********************************************************************
 *
 * This module provides "type-generic" ringbuffers.
 *
 * To use this module, define the `E_RBUF_TYPE`, `E_RBUF_NAME` and
 * `E_RBUF_PREFIX` macros and include the `e_rbuf.h` file for every type you
 * want a ringbuffer for. This will create the type definition and function
 * declarations for a ringbuffer of items of type `E_RBUF_TYPE`. The ringbuffer
 * struct will be named `E_RBUF_NAME` and the ringbuffer functions will be
 * prefixed with `E_RBUF_PREFIX`. When `E_RBUF_IMPL` is defined, the
 * implementations of the ringbuffer functions are generated.
 *  | #define E_RBUF_TYPE int
 *  | #define E_RBUF_NAME Rbuf_Int
 *  | #define E_RBUF_PREFIX rbuf_int
 *  | #define E_RBUF_IMPL
 *  | #include <e_rbuf.h>
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
 *
 ******************************************************************************/

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

/* public interface ***********************************************************/

#ifndef E_RBUF_TYPE
# error "E_RBUF_TYPE not defined"
#endif /* E_RBUF_TYPE */
#ifndef E_RBUF_NAME
# error "E_RBUF_NAME not defined"
#endif /* E_RBUF_NAME */
#ifndef E_RBUF_PREFIX
# error "E_RBUF_PREFIX not defined"
#endif /* E_RBUF_PREFIX */

#define __E_RBUF_CONCAT_HELPER(a, b) a##b
#define __E_RBUF_CONCAT(a, b) __E_RBUF_CONCAT_HELPER (a, b)
#define __E_RBUF_FUNC(suffix) __E_RBUF_CONCAT (E_RBUF_PREFIX, __E_RBUF_CONCAT (_, suffix))

/**
 * The type of the ringbuffer. \ptr points to the raw data, \len is
 * the number of items currently stored, and \cap is the capacity of
 * the buffer. \head and \tail are indices of \ptr as described in the
 * module description.
 */
typedef struct {
	E_RBUF_TYPE *ptr;
	size_t head;
	size_t tail;
	size_t len;
	size_t cap;
} E_RBUF_NAME;

E_RBUF_NAME __E_RBUF_FUNC (init) (E_RBUF_TYPE *buf, size_t cap);
size_t __E_RBUF_FUNC (len) (const E_RBUF_NAME *rbuf);
size_t __E_RBUF_FUNC (remaining_cap) (const E_RBUF_NAME *rbuf);
bool __E_RBUF_FUNC (is_empty) (const E_RBUF_NAME *rbuf);
bool __E_RBUF_FUNC (is_full) (const E_RBUF_NAME *rbuf);
void __E_RBUF_FUNC (reset) (E_RBUF_NAME *rbuf);
bool __E_RBUF_FUNC (try_push) (E_RBUF_NAME *rbuf, E_RBUF_TYPE item);
bool __E_RBUF_FUNC (try_push_back) (E_RBUF_NAME *rbuf, E_RBUF_TYPE item);
void __E_RBUF_FUNC (push) (E_RBUF_NAME *rbuf, E_RBUF_TYPE item);
void __E_RBUF_FUNC (push_back) (E_RBUF_NAME *rbuf, E_RBUF_TYPE item);
bool __E_RBUF_FUNC (pop) (E_RBUF_NAME *rbuf, E_RBUF_TYPE *item_out);
bool __E_RBUF_FUNC (pop_front) (E_RBUF_NAME *rbuf, E_RBUF_TYPE *item_out);

/* implementation *************************************************************/

/**
 * Initialise the ringbuffer based on a pre-existing buffer \buf with capacity
 * \cap. The user is required to allocate and free the buffer if necessary.
 */
E_RBUF_NAME
__E_RBUF_FUNC (init) (E_RBUF_TYPE *buf, size_t cap)
{
	assert (buf != NULL && "buf for e_rbuf_init cannot be null");

	return (E_RBUF_NAME) {
		.ptr = buf,
		.head = 0,
		.tail = 0,
		.len = 0,
		.cap = cap,
	};
}

/**
 * Get the number of items in the ringbuffer \rbuf.
 */
size_t
__E_RBUF_FUNC (len) (const E_RBUF_NAME *rbuf)
{
	return rbuf->len;
}

/**
 * Get the remaining capacity of the ringbuffer \rbuf.
 */
size_t
__E_RBUF_FUNC (remaining_cap) (const E_RBUF_NAME *rbuf)
{
	return rbuf->cap - rbuf->len;
}

/**
 * Check if the ringbuffer \rbuf is empty.
 */
bool
__E_RBUF_FUNC (is_empty) (const E_RBUF_NAME *rbuf)
{
	return rbuf->len == 0;
}

/**
 * Check if the ringbuffer \rbuf is full.
 */
bool
__E_RBUF_FUNC (is_full) (const E_RBUF_NAME *rbuf)
{
	return rbuf->len == rbuf->cap;
}

/**
 * Reset the ringbuffer \rbuf, i.e. "remove" all items.
 */
void
__E_RBUF_FUNC (reset) (E_RBUF_NAME *rbuf)
{
	if (!rbuf) return;
	rbuf->len = 0;
	rbuf->head = 0;
	rbuf->tail = 0;
}

/**
 * Add \item to the front of the ringbuffer \rbuf. If the ringbuffer is full,
 * the tail of the ringbuffer will be overwritten.
 */
void
__E_RBUF_FUNC (push) (E_RBUF_NAME *rbuf, E_RBUF_TYPE item)
{
	if (!rbuf) return;

	if (rbuf->len == rbuf->cap) {
		rbuf->tail = (rbuf->tail + 1) % rbuf->cap;
	} else {
		rbuf->len += 1;
	}

	rbuf->ptr[rbuf->head] = item;
	rbuf->head = (rbuf->head + 1) % rbuf->cap;
}

/**
 * Add \item to the back of the ringbuffer \rbuf. If the ringbuffer is full, the
 * head of the ringbuffer will be overwritten.
 */
void
__E_RBUF_FUNC (push_back) (E_RBUF_NAME *rbuf, E_RBUF_TYPE item)
{
	if (!rbuf) return;

	if (rbuf->len == rbuf->cap) {
		rbuf->head = (rbuf->head + rbuf->cap - 1) % rbuf->cap;
	} else {
		rbuf->len += 1;
	}

	rbuf->tail = (rbuf->tail + rbuf->cap - 1) % rbuf->cap;
	rbuf->ptr[rbuf->tail] = item;
}

/**
 * Try to add \item to the front of the ringbuffer \rbuf. If the ringbuffer is
 * full, no action will be performed and `false` will be returned. Otherwise,
 * the item will be added and `true` is returned.
 */
bool
__E_RBUF_FUNC (try_push) (E_RBUF_NAME *rbuf, E_RBUF_TYPE item)
{
	if (!rbuf) return false;
	if (rbuf->len == rbuf->cap) return false;

	rbuf->len += 1;
	rbuf->ptr[rbuf->head] = item;
	rbuf->head = (rbuf->head + 1) % rbuf->cap;
	return true;
}

/**
 * Try to add \item to the back of the ringbuffer \rbuf. If the ringbuffer is
 * full, no action will be performed and `false` will be returned. Otherwise,
 * the item will be added and `true` is returned.
 */
bool
__E_RBUF_FUNC (try_push_back) (E_RBUF_NAME *rbuf, E_RBUF_TYPE item)
{
	if (!rbuf) return false;
	if (rbuf->len == rbuf->cap) return false;

	rbuf->len += 1;
	rbuf->tail = (rbuf->tail + rbuf->cap - 1) % rbuf->cap;
	rbuf->ptr[rbuf->tail] = item;
	return true;
}

/**
 * Try to pop an item from the back of the ringbuffer \rbuf. If the ringbuffer
 * is not empty, the item will be removed and written to \item_out, and `true`
 * will be returned. If the ringbuffer is empty, no action will be performed,
 * and `false` will be returned. If the \item_out parameter is `nullptr`,
 * nothing will be written to it, but the item will still be popped and `true`
 * or `false` will be returned.
 */
bool
__E_RBUF_FUNC (pop) (E_RBUF_NAME *rbuf, E_RBUF_TYPE *item_out)
{
	if (!rbuf) return false;
	if (rbuf->len == 0) return false;

	rbuf->len -= 1;
	rbuf->head = (rbuf->head + rbuf->cap - 1) % rbuf->cap;
	if (item_out) *item_out = rbuf->ptr[rbuf->head];
	return true;
}

/**
 * Try to pop an item from the front of the ringbuffer \rbuf. If the ringbuffer
 * is not empty, the item will be removed and written to \item_out, and `true`
 * will be returned. If the ringbuffer is empty, no action will be performed,
 * and `false` will be returned. If the \item_out parameter is `nullptr`,
 * nothing will be written to it, but the item will still be popped and `true`
 * or `false` will be returned.
 */
bool
__E_RBUF_FUNC (pop_front) (E_RBUF_NAME *rbuf, E_RBUF_TYPE *item_out)
{
	if (!rbuf) return false;
	if (rbuf->len == 0) return false;

	rbuf->len -= 1;
	if (item_out) *item_out = rbuf->ptr[rbuf->tail];
	rbuf->tail = (rbuf->tail + 1) % rbuf->cap;
	return true;
}

/* undefs *********************************************************************/

#undef __E_RBUF_CONCAT
#undef __E_RBUF_CONCAT_HELPER
#undef __E_RBUF_FUNC
#undef E_RBUF_TYPE
#undef E_RBUF_NAME
#undef E_RBUF_PREFIX
