#ifndef _EMPOWER_VEC_H_
#define _EMPOWER_VEC_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_VEC

/*! e_vec *********************************************************************
 * 
 * This module provides "type-generic" vectors / dynamic arrays. The vectors
 * provided by this module allocate their data using `e_alloc`, i.e. `malloc`.
 * Like in `e_alloc`, when an allocation fails, the programme is terminated.
 *
 * Module dependencies:
 *  - e_alloc
 *  - e_mem
 *  - e_debug (transient)
 *  - e_log (transient; optional)
 *
 ******************************************************************************/

#if !E_CONFIG_MODULE_ALLOC || !E_CONFIG_MODULE_MEM
# error "module e_vec depends on: e_alloc, e_mem"
#endif /* !E_CONFIG_MODULE_ALLOC || !E_CONFIG_MODULE_MEM */

#include <string.h>
#include "convey.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_mem.h"

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
	type_name prefix##_init (void);                                        \
	type_name prefix##_init_with_cap (usize cap);                          \
	type_name prefix##_from (const T *ptr, usize len);                     \
	type_name prefix##_from_allocated (T *ptr, usize len, usize cap);      \
	type_name prefix##_repeat (T item, usize n);                           \
	type_name prefix##_repeat_slice (const T *slice, usize len, usize rep);\
	void prefix##_deinit (type_name *vec);                                 \
	void prefix##_grow (type_name *vec, usize cap);                        \
	void prefix##_clear (type_name *vec);                                  \
	void prefix##_trunc (type_name *vec, usize n);                         \
	type_name prefix##_clone (type_name *vec);                             \
	void prefix##_append (type_name *vec, T item);                         \
	void prefix##_append_slice (type_name *vec, const T *slice, usize slice_len); \
	void prefix##_pop (type_name *vec, T **item_out);                      \
	usize prefix##_pop_slice (type_name *vec, T **slice_out, usize max_len); \
	const T *prefix##_find (const type_name *vec, T item);                 \
	isize prefix##_find_idx (const type_name *vec, T item);                \
	const T *prefix##_find_slice (const type_name *vec, const T *slice, usize len); \
	isize prefix##_find_slice_idx (const type_name *vec, const T *slice, usize len); \
	bool prefix##_contains (const type_name *vec, T item);                 \
	bool prefix##_contains_slice (const type_name *vec, const T *slice, usize len); \
	const T *prefix##_get (const type_name *vec, usize idx);               \
	bool prefix##_set (const type_name *vec, usize idx, T item);           \
	bool prefix##_set_slice (const type_name *vec, usize idx, const T *slice, usize len); \
	void prefix##_insert (type_name *vec, usize idx, T item);              \
	void prefix##_insert_slice (type_name *restrict vec, usize idx, const T *restrict slice, usize len); \
	usize prefix##_remove (type_name *vec, usize idx, usize count);        \
	usize prefix##_count (const type_name *vec, T item);                   \
	usize prefix##_count_slice (const type_name *vec, const T *slice, usize len); \
	usize prefix##_count_slice_overlap (const type_name *vec, const T *slice, usize len); \
                                                                               \
        /* ensure that ; must follow macro invokation: */                      \
	static_assert (true, "")

/**
 * Implement the functions required for a vector of type \T with type \type_name
 * and function prefix \prefix. This goes into a `.c` file and requires there to
 * be a declaration somewhere in a header file with `E_VEC_DECLARE`.
 */
#define E_VEC_IMPLEMENT(T, type_name, prefix)                                  \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with capacity 0. This function does not perform \
	 * an allocation. The vector must be freed by the user using           \
	 * `e_vec_deinit`.                                                     \
	 */                                                                    \
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
	/**                                                                    \
	 * Initialise a vector with a given capacity. This function allocates  \
	 * enough memory to store \cap items. The vector must be freed by the  \
	 * user using `e_vec_deinit`.                                          \
	 */                                                                    \
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
	/**                                                                    \
	 * Create a vector from a region of memory pointed to by \ptr with     \
	 * length \len. If \ptr is `nullptr`, an empty vector is initialized   \
	 * regardless of \len. The memory from \ptr is copied into a newly     \
	 * allocated buffer.                                                   \
	 */                                                                    \
	type_name                                                              \
	prefix##_from (const T *ptr, usize len)                                \
	{                                                                      \
		return (type_name) {                                           \
			.ptr = e_mem_clone (ptr, T, len),                      \
			.len = len,                                            \
			.cap = len,                                            \
		};                                                             \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Create a vector from a pointer \ptr, a length \len and the capacity \
	 * \cap. \ptr must be allocated on the heap using `e_alloc` and it     \
	 * must be capable of storing \cap items. \len is the number of items  \
	 * actually stored in \ptr.                                            \
	 */                                                                    \
	type_name                                                              \
	prefix##_from_allocated (T *ptr, usize len, usize cap)                 \
	{                                                                      \
		return (type_name) {                                           \
			.ptr = ptr,                                            \
			.len = len,                                            \
			.cap = cap,                                            \
		};                                                             \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with \n repeated items with value \item.        \
	 */                                                                    \
	type_name                                                              \
	prefix##_repeat (T item, usize n)                                      \
	{                                                                      \
		T *ptr;                                                        \
		usize i;                                                       \
                                                                               \
		if (n == 0) return prefix##_init ();                           \
                                                                               \
		ptr = e_alloc (T, n);                                          \
		for (i = 0; i < n; i++) {                                      \
			ptr[i] = item;                                         \
		}                                                              \
		                                                               \
		return prefix##_from_allocated (ptr, n, n);                    \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Initialise a vector with a slice \slice of length \len repeated     \
	 * \rep times. If \slice is `nullptr`, 0 repetitions are put into the  \
	 * vector, regardless of \len.                                         \
	 */                                                                    \
	type_name                                                              \
	prefix##_repeat_slice (const T *slice, usize len, usize rep)           \
	{                                                                      \
		T *ptr;                                                        \
		usize i;                                                       \
                                                                               \
		if (!slice || len == 0 || rep == 0) return prefix##_init ();   \
                                                                               \
		ptr = e_alloc (T, len * rep);                                  \
		for (i = 0; i < rep; i++) {                                    \
			e_mem_copy (&ptr[i * len], slice, T, len);             \
		}                                                              \
		                                                               \
		return prefix##_from_allocated (ptr, len * rep, len * rep);    \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Deinitialise a vector and free the memory occupied by it. If the    \
	 * vector is used for storing allocated pointers to data, the user     \
	 * must free this memory, as it is not done here.                      \
	 */                                                                    \
	void                                                                   \
	prefix##_deinit (type_name *vec)                                       \
	{                                                                      \
		if (vec) {                                                     \
			e_free (vec->ptr);                                     \
		}                                                              \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Grow the memory of a vector \vec to at least \cap items. If \cap is \
	 * lower than the current capacity of the vector, no action will be    \
	 * performed. Internally, it rounds up \cap to the next power of two   \
	 * and reallocates the memory to the new capacity.                     \
	 */                                                                    \
	void                                                                   \
	prefix##_grow (type_name *vec, usize cap)                              \
	{                                                                      \
		if (!vec) return;                                              \
		if (cap <= vec->cap) return;                                   \
		vec->cap = stdc_bit_ceil (cap);                                \
		vec->ptr = e_realloc (vec->ptr, T, vec->cap);                  \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Clear all items from a vector \vec. The memory is freed. This is    \
	 * the same as calling `e_vec_deinit` followed by `e_vec_init`.        \
	 */                                                                    \
	void                                                                   \
	prefix##_clear (type_name *vec)                                        \
	{                                                                      \
		if (!vec) return;                                              \
		prefix##_deinit (vec);                                         \
		*vec = prefix##_init ();                                       \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Truncate a vector \vec to \n items. The memory is reallocated (i.e. \
	 * shrank) to fit the new number of items. If \n is larger than the    \
	 * length of the vector, no action is performed.                       \
	 */                                                                    \
	void                                                                   \
	prefix##_trunc (type_name *vec, usize n)                               \
	{                                                                      \
		if (!vec) return;                                              \
		if (vec->len >= n) return;                                     \
		vec->len = n;                                                  \
		vec->cap = stdc_bit_ceil (n);                                  \
		vec->ptr = e_realloc (vec->ptr, T, n);                         \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Clone a vector. This function allocates memory which must be freed  \
	 * by the user using \e_vec_deinit. If \vec is `nullptr`, an empty     \
	 * vector is returned.                                                 \
	 */                                                                    \
	type_name                                                              \
	prefix##_clone (type_name *vec)                                        \
        {                                                                      \
		T *ptr;                                                        \
                                                                               \
		if (!vec) return prefix##_init ();                             \
                                                                               \
		ptr = e_alloc (T, vec->cap);                                   \
		e_mem_copy (ptr, vec->ptr, T, vec->len);                       \
		                                                               \
		return (type_name) {                                           \
			.ptr = ptr,                                            \
			.cap = vec->cap,                                       \
			.len = vec->len,                                       \
		};                                                             \
        }                                                                      \
                                                                               \
	/**                                                                    \
	 * Add the item \item to the end of the vector \vec. If the vector     \
	 * does not have enough capacity, its data will be reallocated to      \
	 * accommodate for the new item.                                       \
	 */                                                                    \
	void                                                                   \
	prefix##_append (type_name *vec, T item)                               \
	{                                                                      \
		prefix##_append_slice (vec, &item, 1);                         \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Adds \slice_len items from the memory location \slice to the end of \
	 * the vector \vec. If the vector does not have enough capacity, its   \
	 * data will be reallocated to accommodate for the new item.           \
	 */                                                                    \
	void                                                                   \
	prefix##_append_slice (type_name *vec, const T *slice, usize slice_len)\
	{                                                                      \
		if (!vec) return;                                              \
		if (vec->len + slice_len > vec->cap) {                         \
			prefix##_grow (vec, vec->len + slice_len);             \
		}                                                              \
		e_mem_copy (&vec->ptr[vec->len], slice, T, slice_len);         \
		vec->len += slice_len;                                         \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Pop the last item from the vector \vec. The item will be stored in  \
	 * \item_out. If \vec is empty or `nullptr`, \item_out will be set to  \
	 * `nullptr`. The length field of the vector will be decremented. If   \
	 * \item_out is `nullptr`, the item will be popped and the length      \
	 * field will be updated, but \item_out will not be filled. The memory \
	 * of \item_out will remain valid until the vector buffer is           \
	 * reallocated. If the vector is used for storing allocated pointers   \
	 * to data, the user must free this memory, as it is not done here.    \
	 */                                                                    \
	void                                                                   \
	prefix##_pop (type_name *vec, T **item_out)                            \
	{                                                                      \
		if (!vec) {                                                    \
			if (item_out) *item_out = nullptr;                     \
			return;                                                \
		}                                                              \
                                                                               \
		if (vec->len > 0) {                                            \
			vec->len -= 1;                                         \
			if (item_out) *item_out = &vec->ptr[vec->len];         \
		} else {                                                       \
			if (item_out) *item_out = nullptr;                     \
		}                                                              \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Pop the last items from the vector \vec. \max_len represents the    \
	 * maxmimum number of items that should be popped. The number of items \
	 * which were actually popped is returned. \slice_out will be set to   \
	 * point to the first of the popped items. If \vec is empty or         \
	 * `nullptr`, \slice_out will be set to `nullptr` and 0 will be        \
	 * returned. The length field of the vector will be decremented by the \
	 * number of popped items. If \slice_out is `nullptr`, the items will  \
	 * be popped, the length field will be updated and the number of       \
	 * popped items will be returned, but \slice_out will not be filled.   \
	 * The memory of \slice_out will remain valid until the vector buffer  \
	 * is reallocated. If the vector is used for storing allocated         \
	 * pointers to data, the user must free this memory, as it is not done \
	 * here.                                                               \
	 */                                                                    \
	usize                                                                  \
	prefix##_pop_slice (type_name *vec, T **slice_out, usize max_len)      \
	{                                                                      \
		usize slice_len;                                               \
                                                                               \
		if (!vec) {                                                    \
			if (slice_out) *slice_out = nullptr;                   \
			return 0;                                              \
		}                                                              \
                                                                               \
		if (vec->len > 0) {                                            \
			slice_len = vec->len < max_len ? vec->len : max_len;   \
			vec->len -= slice_len;                                 \
			if (slice_out) *slice_out = &vec->ptr[vec->len];       \
			return slice_len;                                      \
		} else {                                                       \
			if (slice_out) *slice_out = nullptr;                   \
			return 0;                                              \
		}                                                              \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Find the position of \item in the vector \vec. In order to find the \
	 * item, `memcmp` between \item and the items in the vector is used.   \
	 * Returns a pointer to the item. If you want to get the index instead \
	 * of the pointer, use `e_vec_find_idx`. If \item is not found or \vec \
	 * is `nullptr`, `nullptr` is returned.                                \
	 */                                                                    \
	const T *                                                              \
	prefix##_find (const type_name *vec, T item)                           \
	{                                                                      \
		isize idx;                                                     \
                                                                               \
		idx = prefix##_find_idx (vec, item);                           \
		if (idx < 0) return nullptr;                                   \
		return (const T *) &vec->ptr[idx];                             \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Find the position of \item in the vector \vec. In order to find the \
	 * item, `memcmp` between \item and the items in the vector is used.   \
	 * Returns the index of the item. If you want to get the pointer       \
	 * instead of the index, use `e_vec_find`. If \item is not found or    \
	 * \vec is `nullptr`, -1 is returned.                                  \
	 */                                                                    \
	isize                                                                  \
	prefix##_find_idx (const type_name *vec, T item)                       \
	{                                                                      \
		usize i;                                                       \
                                                                               \
		if (!vec) return -1;                                           \
		for (i = 0; i < vec->len; i++) {                               \
			if (!memcmp (&vec->ptr[i], &item, sizeof (T))) {       \
				return (isize) i;                              \
			}                                                      \
		}                                                              \
		return -1;                                                     \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Find the position of a slice of memory \slice with length \len in   \
	 * the vector \vec. In order to find the items, `memcmp` between       \
	 * \slice and the items in the vector is used. Returns a pointer to    \
	 * the first item of the matched slice. If you want to get the index   \
	 * instead of the pointer, use `e_vec_find_slice_idx`. If \item is     \
	 * not found or \vec is `nullptr`, `nullptr` is returned. If \slice is \
	 * `nullptr` or \len is 0 and \vec is has at least one element, a      \
	 * pointer to that element is returned.                                \
	 */                                                                    \
	const T *                                                              \
	prefix##_find_slice (const type_name *vec, const T *slice, usize len)  \
	{                                                                      \
		isize idx;                                                     \
                                                                               \
		idx = prefix##_find_slice_idx (vec, slice, len);               \
		if (idx < 0) return nullptr;                                   \
		return (const T *) &vec->ptr[idx];                             \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Find the position of a slice of memory \slice with length \len in   \
	 * the vector \vec. In order to find the items, `memcmp` between       \
	 * \slice and the items in the vector is used. Returns the index of    \
	 * the first item of the matched slice. If you want to get the pointer \
	 * instead of the index of the first item, use `e_vec_find_slice`. If  \
	 * \item is not found or \vec is `nullptr`, `nullptr` is returned. If  \
	 * \slice is `nullptr` or \len is 0 and \vec has at least one element, \
	 * the first element's index (i.e. 0) is returned, -1 otherwise.       \
	 */                                                                    \
	isize                                                                  \
	prefix##_find_slice_idx (const type_name *vec, const T *slice,         \
	                         usize len)                                    \
	{                                                                      \
		usize i;                                                       \
                                                                               \
		if (!vec) return -1;                                           \
		if (!slice || len == 0) return vec->len == 0 ? -1 : 0;         \
		for (i = 0; i < vec->len - len + 1; i++) {                     \
			if (!memcmp (&vec->ptr[i], slice, sizeof (T) * len)) { \
				return (isize) i;                              \
			}                                                      \
		}                                                              \
		return -1;                                                     \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Check if the vector \vec contains the item \item. For comparing the \
	 * items in \vec with \item, `memcmp` is used. Returns `true` if the   \
	 * item is found, `false` otherwise. If \vec is `nullptr`, `false` is  \
	 * returned.                                                           \
	 */                                                                    \
	bool                                                                   \
	prefix##_contains (const type_name *vec, T item)                       \
	{                                                                      \
		return prefix##_find_idx (vec, item) >= 0;                     \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Check if the vector \vec contains the slice \slice of length \len.  \
	 * For comparing the items in \vec with \slice, `memcmp` is used.      \
	 * Returns `true` if the slice is found, `false` otherwise. If \vec is \
	 * `nullptr`, `false` is returned. If \slice is `nullptr` or \len is   \
	 * 0, `true` is returned.                                              \
	 */                                                                    \
	bool                                                                   \
	prefix##_contains_slice (const type_name *vec, const T *slice,         \
	                         usize len)                                    \
	{                                                                      \
		return prefix##_find_slice_idx (vec, slice, len) >= 0;         \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Get the item at index \idx of the vector \vec. Returns a poitner to \
	 * the item. If \vec is `nullptr` or \idx is out of range, `nullptr`   \
	 * is returned.                                                        \
	 */                                                                    \
	const T *                                                              \
	prefix##_get (const type_name *vec, usize idx)                         \
	{                                                                      \
		if (!vec) return nullptr;                                      \
		if (idx >= vec->len) return nullptr;                           \
		return (const T *) &vec->ptr[idx];                             \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Set the item at index \idx of the vector \vec to \item. When \idx   \
	 * is out of range, the item is not set. No reallocation is performed. \
	 * When the item was set, `true` is returned. When the item was not    \
	 * set or \vec is `nullptr`, `false` is returned.                      \
	 */                                                                    \
	bool                                                                   \
	prefix##_set (const type_name *vec, usize idx, T item)                 \
	{                                                                      \
		if (!vec) return false;                                        \
		if (idx >= vec->len) return false;                             \
		vec->ptr[idx] = item;                                          \
		return true;                                                   \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Set the items beginning at index \idx of the vector \vec to the     \
	 * slice \slice of length \len. When \idx is out of range, the items   \
	 * are not set. No reallocation is performed. When the items were set, \
	 * `true` is returned. When the items were not set or \vec is          \
	 * `nullptr`, `false` is returned.                                     \
	 */                                                                    \
	bool                                                                   \
	prefix##_set_slice (const type_name *vec, usize idx, const T *slice,   \
	                    usize len)                                         \
	{                                                                      \
		if (!vec) return false;                                        \
		if (idx + len - 1 >= vec->len) return false;                   \
		e_mem_copy (&vec->ptr[idx], slice, T, len);                    \
		return true;                                                   \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Insert \item at the position \idx into the vector \vec. The rest of \
	 * the vector is shifted backwards. If \idx is out of range, the item  \
	 * is appended to the vector. The memory is reallocated if necessary.  \
	 */                                                                    \
	void                                                                   \
	prefix##_insert (type_name *vec, usize idx, T item)                    \
	{                                                                      \
		prefix##_insert_slice (vec, idx, &item, 1);                    \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Insert a slice \slice of \len items at position \idx into the       \
	 * vector \vec. The rest of the vector is shifted backwards. If \idx   \
	 * is out of range, the items are appended to the vector. The memory   \
	 * is reallocated if necessary.                                        \
	 */                                                                    \
	void                                                                   \
	prefix##_insert_slice (type_name *restrict vec, usize idx,             \
	                       const T *restrict slice, usize len)             \
	{                                                                      \
		if (!vec || !slice || len == 0) return;                        \
		if (idx >= vec->len) {                                         \
			prefix##_append_slice (vec, slice, len);               \
			return;                                                \
		}                                                              \
		if (vec->len >= vec->cap) {                                    \
			prefix##_grow (vec, vec->cap + len);                   \
		}                                                              \
		e_mem_move (vec->ptr + idx + len, vec->ptr + idx, T,           \
			   (vec->len - idx));                                  \
		e_mem_copy (vec->ptr + idx, slice, T, len);                    \
		vec->len += len;                                               \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Remove \count items from the vector \vec starting at position \idx. \
	 * No action is performed for items at positions which would be out of \
	 * bounds. When removing memory in the middle of the vector, the       \
	 * memory is shifted accordingly. Returns the number of removed        \
	 * elements.                                                           \
	 */                                                                    \
	usize                                                                  \
	prefix##_remove (type_name *vec, usize idx, usize count)               \
	{                                                                      \
		if (!vec || idx >= vec->len || count == 0) return 0;           \
		if (idx + count == vec->len) {                                 \
			vec->len -= count;                                     \
			return count;                                          \
		}                                                              \
		count = idx + count > vec->len ? vec->len - idx : count;       \
		e_mem_move (vec->ptr + idx, vec->ptr + idx + count, T, count); \
		vec->len -= count;                                             \
		return count;                                                  \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Count the number of occurances of \item in the vector \vec. If \vec \
	 * is `nullptr`, 0 is returned. For comparing the items, `memcmp` is   \
	 * used.                                                               \
	 */                                                                    \
	usize                                                                  \
	prefix##_count (const type_name *vec, T item)                          \
	{                                                                      \
		return prefix##_count_slice_overlap (vec, &item, 1);           \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Count the number of occurances of a slice \slice of length \len     \
	 * in the vector \vec. If \vec is `nullptr`, 0 is returned. For        \
	 * comparing the items, `memcmp` is used. Overlap is not counted.      \
	 */                                                                    \
	usize                                                                  \
	prefix##_count_slice (const type_name *vec, const T *slice, usize len) \
	{                                                                      \
		usize i, count;                                                \
                                                                               \
		if (!vec) return 0;                                            \
		count = 0;                                                     \
		for (i = 0; i < vec->len; ) {                                  \
			if (!memcmp (&vec->ptr[i], slice, sizeof (T) * len)) { \
				count += 1;                                    \
				i += len;                                      \
			} else {                                               \
				i += 1;                                        \
			}                                                      \
		}                                                              \
                                                                               \
		return count;                                                  \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Count the number of occurances of a slice \slice of length \len     \
	 * in the vector \vec. If \vec is `nullptr`, 0 is returned. For        \
	 * comparing the items, `memcmp` is used. Overlap is counted.          \
	 */                                                                    \
	usize                                                                  \
	prefix##_count_slice_overlap (const type_name *vec, const T *slice,    \
	                              usize len)                               \
	{                                                                      \
		usize i, count;                                                \
                                                                               \
		if (!vec) return 0;                                            \
		count = 0;                                                     \
		for (i = 0; i < vec->len; i++) {                               \
			if (!memcmp (&vec->ptr[i], slice, sizeof (T) * len)) { \
				count += 1;                                    \
			}                                                      \
		}                                                              \
                                                                               \
		return count;                                                  \
	}                                                                      \
                                                                               \
        /* ensure that ; must follow macro invokation: */                      \
	static_assert (true, "")

#endif /* E_CONFIG_MODULE_VEC */
#endif /* _EMPOWER_VEC_H_ */
