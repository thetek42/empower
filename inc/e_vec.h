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
 * of the vector will be equal to \prefix with `_t` appended to it. All
 * functions will also start with \prefix. For instance, to create a
 * vector with items of type `Node *`, use something like the following snippet:
 *
 * | E_VEC_DECLARE (Node *, E_Vec_Node_Ptr, e_vec_node_ptr);
 * | E_VEC_IMPLEMENT (Node *, E_Vec_Node_Ptr, e_vec_node_ptr);
 * |
 * | e_vec_node_ptr_t nodes;
 * | nodes = e_vec_node_ptr_init ();
 * | e_vec_node_ptr_deinit (&nodes);
 *
 * @param T: The type of the vector data items
 * @param type_name: The name that the vector type should have
 * @param prefix: The prefix for all functions
 * @node This should go into a `.h` file.
 * @see \E_VEC_IMPLEMENT
 */
/* docgen: define(type_name, E_Vec) */
/* docgen: define(prefix, e_vec) */
#define E_VEC_DECLARE(T, type_name, prefix)                                    \
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
	} type_name;                                                           \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with capacity 0. This function does not perform \
	 * an allocation.                                                      \
	 *                                                                     \
	 * @return The new vector                                              \
	 */                                                                    \
	type_name prefix##_init (void);                                        \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with a given capacity. This function allocates  \
	 * enough memory to store \cap items.                                  \
	 *                                                                     \
	 * @param cap: The capacity of the vector                              \
	 * @return The new vector.                                             \
	 */                                                                    \
	type_name prefix##_init_with_cap (usize cap);                          \
                                                                               \
	/**                                                                    \
	 * Deinitialise a vector and free the memory occupied by it.           \
	 *                                                                     \
	 * @param vec: The vector to deinitialise                              \
	 */                                                                    \
	void prefix##_deinit (type_name *vec);                                 \
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
	void prefix##_grow (type_name *vec, usize cap);                        \
                                                                               \
	/**                                                                    \
	 * Clone a vector. This function allocates memory which must be freed  \
	 * by the user using \e_vec_free. If \vec is `nullptr`, an empty       \
	 * vector is returned.                                                 \
	 *                                                                     \
	 * @param vec: The vector to clone                                     \
	 * @return A new vector                                                \
	 */                                                                    \
	type_name prefix##_clone (type_name *vec);                             \
                                                                               \
	/**                                                                    \
	 * Add the item \item to the end of the vector \vec. If the vector     \
	 * does not have enough capacity, its data will be reallocated to      \
	 * accommodate for the new item.                                       \
	 *                                                                     \
	 * @param vec: The vector to append to                                 \
	 * @param item: The item to add                                        \
	 */                                                                    \
	void prefix##_append (type_name *vec, T item);                         \
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
	void prefix##_append_slice (type_name *vec, const T *slice,            \
	                            usize slice_len);                          \
                                                                               \
	static_assert (true, "")
/* docgen: undef(type_name) */
/* docgen: undef(prefix) */

/**
 * Implement the functions required for a vector of type \T with type and
 * function prefix \prefix.
 *
 * @param T: The type of the vector data items
 * @param type_name: The name that the vector type should have
 * @param prefix: The prefix for all functions
 * @node This should go into a `.c` file.
 * @see \E_VEC_DECLARE
 */
/* docgen: define(type_name, E_Vec) */
/* docgen: define(prefix, e_vec) */
#define E_VEC_IMPLEMENT(T, type_name, prefix)                                  \
                                                                               \
	type_name                                                              \
	prefix##_init (void)                                                   \
	{                                                                      \
		return (type_name) {                                           \
			.ptr = nullptr,                                        \
			.len = 0,                                              \
			.cap = 0,                                              \
		};                                                             \
	}                                                                      \
                                                                               \
	type_name                                                              \
	prefix##_init_with_cap (usize cap)                                     \
	{                                                                      \
		return (type_name) {                                           \
			.ptr = e_alloc (T, cap),                               \
			.len = 0,                                              \
			.cap = cap,                                            \
		};                                                             \
	}                                                                      \
                                                                               \
	void                                                                   \
	prefix##_deinit (type_name *vec)                                       \
	{                                                                      \
		if (vec) {                                                     \
			e_free (vec->ptr);                                     \
		}                                                              \
	}                                                                      \
                                                                               \
	void                                                                   \
	prefix##_grow (type_name *vec, usize cap)                              \
	{                                                                      \
		if (!vec) return;                                              \
		if (cap <= vec->cap) return;                                   \
		vec->cap = stdc_bit_ceil (cap);                                \
		vec->ptr = e_realloc (vec->ptr, T, vec->cap);                  \
	}                                                                      \
                                                                               \
	type_name                                                              \
	prefix##_clone (type_name *vec);                                       \
        {                                                                      \
		T *ptr;                                                        \
                                                                               \
		if (!vec) return prefix##_init ();                             \
                                                                               \
		ptr = e_alloc (vec->cap);                                      \
		memcpy (ptr, vec->ptr, vec->len);                              \
		                                                               \
		return (type_name) {                                           \
			.ptr = ptr,                                            \
			.cap = vec->cap,                                       \
			.len = vec->len,                                       \
		};                                                             \
        }                                                                      \
                                                                               \
	void                                                                   \
	prefix##_append (type_name *vec, T item)                               \
	{                                                                      \
		if (!vec) return;                                              \
		if (vec->len >= vec->cap) {                                    \
			prefix##_grow (vec, vec->cap + 1);                     \
		}                                                              \
		vec->ptr[vec->len] = item;                                     \
		vec->len += 1;                                                 \
	}                                                                      \
                                                                               \
	void                                                                   \
	prefix##_append_slice (type_name *vec, const T *slice, usize slice_len)\
	{                                                                      \
		if (!vec) return;                                              \
		if (vec->len + slice_len > vec->cap) {                         \
			prefix##_grow (vec, vec->cap + slice_len);             \
		}                                                              \
		memcpy (&vec->ptr[vec->len], slice, slice_len * sizeof (T));   \
		vec->len += slice_len;                                         \
	}                                                                      \
                                                                               \
	static_assert (true, "")
/* docgen: undef(type_name) */
/* docgen: undef(prefix) */

#endif /* E_CONFIG_MODULE_VEC */
#endif /* _EMPOWER_VEC_H_ */
