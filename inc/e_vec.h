#ifndef _EMPOWER_VEC_H_
#define _EMPOWER_VEC_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_VEC

#if !E_CONFIG_MODULE_ALLOC
# error "module e_alloc depends on: e_alloc"
#endif /* !E_CONFIG_MODULE_ALLOC */

/*! e_vec *********************************************************************
 * 
 * This module provides "type-generic" vectors / dynamic arrays. The vectors
 * provided by this module allocate their data using `e_alloc`, i.e. `malloc`.
 * Like in `e_alloc`, when an allocation fails, the programme is terminated.
 *
 * Module dependencies:
 *  - e_alloc
 *  - e_log (transient)
 *
 ******************************************************************************/

/**
 * Declare the struct and functions required for a vector of type \T. The type
 * of the vector will be equal to \vec_type_prefix with `_t` appended to it. All
 * functions will also start with \vec_type_prefix. For instance, to create a
 * vector of type `node_t *`, use something like the following snippet:
 *
 * | E_VEC_DECLARE (node_t *, e_vec_node_ptr);
 * | E_VEC_IMPLEMENT (node_t *, e_vec_node_ptr);
 * |
 * | e_vec_node_ptr_t nodes;
 * | nodes = e_vec_node_ptr_init ();
 * | e_vec_node_ptr_deinit (&nodes);
 *
 * @param T: The type of the vector data items
 * @param vec_type_prefix: The prefix for the vector type and all functions
 * @node This should go into a `.h` file.
 * @see \E_VEC_IMPLEMENT
 */
/* docgen: define(vec_type_prefix, e_vec) */
#define E_VEC_DECLARE(T, vec_type_prefix)                                      \
                                                                               \
	/**                                                                    \
	 * The type of the vector / dynamic array.                             \
	 *                                                                     \
	 * @field ptr: The pointer to the data                                 \
	 * @field len: The number of items in the vector                       \
	 * @field cap: The capacity of the vector, i.e. the number of items it \
	 *             can store before reallocating. `cap` is guaranteed to   \
	 *             be greater or equal than `len`.                         \
	 */                                                                    \
	typedef struct {                                                       \
		T *ptr;                                                        \
		usize len;                                                     \
		usize cap;                                                     \
	} vec_type_prefix##_t;                                                 \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with capacity 0. This function does not perform \
	 * an allocation.                                                      \
	 *                                                                     \
	 * @return The new vector                                              \
	 */                                                                    \
	vec_type_prefix##_t vec_type_prefix##_init (void);                     \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with a given capacity. This function allocates  \
	 * enough memory to store \cap items.                                  \
	 *                                                                     \
	 * @param cap: The capacity of the vector                              \
	 * @return The new vector.                                             \
	 */                                                                    \
	vec_type_prefix##_t vec_type_prefix##_init_with_cap (usize cap);       \
                                                                               \
	/**                                                                    \
	 * Deinitialise a vector and free the memory occupied by it.           \
	 *                                                                     \
	 * @param vec: The vector to deinitialise                              \
	 */                                                                    \
	void vec_type_prefix##_deinit (vec_type_prefix##_t *vec);              \
                                                                               \
	/**                                                                    \
	 * Grow the memory of a vector \vec to at least \cap items. If \cap is \
	 * lower than the current capacity of the vector, no action will be    \
	 * performed. Internally, it rounds up \cap to the next power of two   \
	 * and reallocates the memory to the new capacity.                     \
	 *                                                                     \
	 * @param vec: The vector to grow                                      \
	 * @param cap: The lower bound for the new capacity                    \
	 */                                                                    \
	void vec_type_prefix##_grow (vec_type_prefix##_t *vec, usize cap);     \
                                                                               \
	/**                                                                    \
	 * Add the item \item to the end of the vector \vec. If the vector     \
	 * does not have enough capacity, its data will be reallocated to      \
	 * accommodate for the new item.                                       \
	 *                                                                     \
	 * @param vec: The vector to append to                                 \
	 * @param item: The item to add                                        \
	 */                                                                    \
	void vec_type_prefix##_append (vec_type_prefix##_t *vec, T item);      \
                                                                               \
	/**                                                                    \
	 * Adds \slice_len items from the memory location \slice to the end of \
	 * the vector \vec. If the vector does not have enough capacity, its   \
	 * data will be reallocated to accommodate for the new item.           \
	 *                                                                     \
	 * @param vec: The vector to append to                                 \
	 * @param slice: The pointer to the list of items to add               \
	 * @param slice_len: The number of items to add                        \
	 * @note \slice must have at least \slice_len items.                   \
	 */                                                                    \
	void vec_type_prefix##_append_slice (vec_type_prefix##_t *vec,         \
	                                     const T *slice, usize slice_len); \
                                                                               \
	static_assert (true, "")
/* docgen: undef(vec_type_prefix) */

/**
 * Implement the functions required for a vector of type \T with type and
 * function prefix \vec_type_prefix.
 *
 * @param T: The type of the vector data items
 * @param vec_type_prefix: The prefix for the vector type and all functions
 * @node This should go into a `.c` file.
 * @see \E_VEC_DECLARE
 */
/* docgen: define(vec_type_prefix, e_vec) */
#define E_VEC_IMPLEMENT(T, vec_type_prefix)                                    \
                                                                               \
	vec_type_prefix##_t                                                    \
	vec_type_prefix##_init (void)                                          \
	{                                                                      \
		vec_type_prefix##_t vec;                                       \
		vec.ptr = nullptr;                                             \
		vec.len = 0;                                                   \
		vec.cap = 0;                                                   \
		return vec;                                                    \
	}                                                                      \
                                                                               \
	vec_type_prefix##_t                                                    \
	vec_type_prefix##_init_with_cap (usize cap)                            \
	{                                                                      \
		vec_type_prefix##_t vec;                                       \
		vec.ptr = e_alloc (T, cap);                                    \
		vec.len = 0;                                                   \
		vec.cap = cap;                                                 \
		return vec;                                                    \
	}                                                                      \
                                                                               \
	void                                                                   \
	vec_type_prefix##_deinit (vec_type_prefix##_t *vec)                    \
	{                                                                      \
		if (vec) {                                                     \
			e_free (vec->ptr);                                     \
		}                                                              \
	}                                                                      \
                                                                               \
	void                                                                   \
	vec_type_prefix##_grow (vec_type_prefix##_t *vec, usize cap)           \
	{                                                                      \
		if (!vec) return;                                              \
		if (cap <= vec->cap) return;                                   \
		vec->cap = stdc_bit_ceil (cap);                                \
		vec->ptr = e_realloc (vec->ptr, T, vec->cap);                  \
	}                                                                      \
                                                                               \
	void                                                                   \
	vec_type_prefix##_append (vec_type_prefix##_t *vec, T item)            \
	{                                                                      \
		if (!vec) return;                                              \
		if (vec->len >= vec->cap) {                                    \
			vec_type_prefix##_grow (vec, vec->cap + 1);            \
		}                                                              \
		vec->ptr[vec->len] = item;                                     \
		vec->len += 1;                                                 \
	}                                                                      \
                                                                               \
	void                                                                   \
	vec_type_prefix##_append_slice (vec_type_prefix##_t *vec,              \
	                                const T *slice, usize slice_len)       \
	{                                                                      \
		if (!vec) return;                                              \
		if (vec->len + slice_len > vec->cap) {                         \
			vec_type_prefix##_grow (vec, vec->cap + slice_len);    \
		}                                                              \
		memcpy (&vec->ptr[vec->len], slice, slice_len * sizeof (T));   \
		vec->len += slice_len;                                         \
	}                                                                      \
                                                                               \
	static_assert (true, "")
/* docgen: undef(vec_type_prefix) */

#endif /* E_CONFIG_MODULE_VEC */
#endif /* _EMPOWER_VEC_H_ */
