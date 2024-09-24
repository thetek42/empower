#ifndef _EMPOWER_VEC_H_
#define _EMPOWER_VEC_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_VEC

#if !E_CONFIG_MODULE_ALLOC
# error "module e_vec depends on: e_alloc"
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
 * of the vector will be equal to \type_name. All functions will start with
 * \prefix. This belongs into a header file and requires the functions to be
 * implemented somewhere with `E_VEC_IMPLEMENT`. For instance, to create a
 * vector with items of type `Node *`, use something like the following snippet:
 *
 * | E_VEC_DECLARE (Node *, E_Vec_Node_Ptr, e_vec_node_ptr);
 * | E_VEC_IMPLEMENT (Node *, E_Vec_Node_Ptr, e_vec_node_ptr);
 * |
 * | E_Vec_Node_Ptr nodes;
 * | nodes = e_vec_node_ptr_init ();
 * | e_vec_node_ptr_deinit (&nodes);
 */
#define E_VEC_DECLARE(T, type_name, prefix)                                    \
                                                                               \
	/**                                                                    \
	 * The type of the vector / dynamic array. \ptr is the pointer to the  \
	 * data, \len is the number of items in the vector and \cap is the     \
	 * capacity, i.e. the number of items it can store before reallocating.\
	 */                                                                    \
	typedef struct {                                                       \
		T *ptr;                                                        \
		usize len;                                                     \
		usize cap;                                                     \
	} type_name;                                                           \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with capacity 0. This function does not perform \
	 * an allocation. The vector must be freed by the user using           \
	 * `e_vec_deinit`.                                                     \
	 */                                                                    \
	type_name prefix##_init (void);                                        \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with a given capacity. This function allocates  \
	 * enough memory to store \cap items. The vector must be freed by the  \
	 * user using `e_vec_deinit`.                                          \
	 */                                                                    \
	type_name prefix##_init_with_cap (usize cap);                          \
                                                                               \
	/**                                                                    \
	 * Deinitialise a vector and free the memory occupied by it.           \
	 */                                                                    \
	void prefix##_deinit (type_name *vec);                                 \
                                                                               \
	/**                                                                    \
	 * Grow the memory of a vector \vec to at least \cap items. If \cap is \
	 * lower than the current capacity of the vector, no action will be    \
	 * performed. Internally, it rounds up \cap to the next power of two   \
	 * and reallocates the memory to the new capacity.                     \
	 */                                                                    \
	void prefix##_grow (type_name *vec, usize cap);                        \
                                                                               \
	/**                                                                    \
	 * Clone a vector. This function allocates memory which must be freed  \
	 * by the user using \e_vec_deinit. If \vec is `nullptr`, an empty     \
	 * vector is returned.                                                 \
	 */                                                                    \
	type_name prefix##_clone (type_name *vec);                             \
                                                                               \
	/**                                                                    \
	 * Add the item \item to the end of the vector \vec. If the vector     \
	 * does not have enough capacity, its data will be reallocated to      \
	 * accommodate for the new item.                                       \
	 */                                                                    \
	void prefix##_append (type_name *vec, T item);                         \
                                                                               \
	/**                                                                    \
	 * Adds \slice_len items from the memory location \slice to the end of \
	 * the vector \vec. If the vector does not have enough capacity, its   \
	 * data will be reallocated to accommodate for the new item.           \
	 */                                                                    \
	void prefix##_append_slice (type_name *vec, const T *slice,            \
	                            usize slice_len);                          \
                                                                               \
	static_assert (true, "")

/**
 * Implement the functions required for a vector of type \T with type \type_name
 * and function prefix \prefix. This goes into a `.c` file and requires there to
 * be a declaration somewhere in a header file with `E_VEC_DECLARE`.
 */
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
	prefix##_clone (type_name *vec)                                        \
        {                                                                      \
		T *ptr;                                                        \
                                                                               \
		if (!vec) return prefix##_init ();                             \
                                                                               \
		ptr = e_alloc (T, vec->cap);                                   \
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

#endif /* E_CONFIG_MODULE_VEC */
#endif /* _EMPOWER_VEC_H_ */
