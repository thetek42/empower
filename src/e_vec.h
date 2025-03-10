#ifndef _EMPOWER_VEC_H_
#define _EMPOWER_VEC_H_

/*! e_vec *********************************************************************
 *
 * This module provides "type-generic" vectors / dynamic arrays. The vectors
 * provided by this module allocate their data using `e_alloc`, i.e. `malloc`.
 * Like in `e_alloc`, when an allocation fails, the programme is terminated.
 *
 * Configuration options:
 *  - `E_CONFIG_MALLOC_FUNC` (default `malloc`): The function to use for allocating memory.
 *  - `E_CONFIG_REALLOC_FUNC` (default `realloc`): The function to use for reallocating memory.
 *  - `E_CONFIG_FREE_FUNC` (default `free`): The function to use for freeing memory.
 *
 ******************************************************************************/

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

/* compatibility annoyances ***************************************************/

#if !defined (__E_SSIZE_T_DEFINED) && defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
# define __E_SSIZE_T_DEFINED
# include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif /* !defined (__E_SSIZE_T_DEFINED) && defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */

/* public interface ***********************************************************/

#ifndef E_CONFIG_MALLOC_FUNC
# define E_CONFIG_MALLOC_FUNC malloc
#endif /* E_CONFIG_MALLOC_FUNC */
#ifndef E_CONFIG_REALLOC_FUNC
# define E_CONFIG_REALLOC_FUNC realloc
#endif /* E_CONFIG_REALLOC_FUNC */
#ifndef E_CONFIG_FREE_FUNC
# define E_CONFIG_FREE_FUNC free
#endif /* E_CONFIG_FREE_FUNC */

#define __e_vec_ensure_user_has_to_use_semicolon() size_t strlen (const char *s)

/**
 * Declare the struct and functions required for a vector of type \T. The type
 * of the vector will be equal to \type_name. All functions will start with
 * \prefix. This belongs into a header file and requires the functions to be
 * implemented somewhere with `E_VEC_IMPL`. For instance, to create a vector
 * with items of type `Node *`, use something like the following snippet:
 *
 *  | #include <e_vec.h>
 *  |
 *  | E_VEC_DECL (Node *, E_Vec_Node_Ptr, e_vec_node_ptr);
 *  | E_VEC_IMPL (Node *, E_Vec_Node_Ptr, e_vec_node_ptr);
 *  |
 *  | E_Vec_Node_Ptr nodes;
 *  | nodes = e_vec_node_ptr_init ();
 *  | e_vec_node_ptr_deinit (&nodes);
 */
#define E_VEC_DECL(T, type_name, prefix)                                       \
                                                                               \
	/**                                                                    \
	 * The type of the vector / dynamic array. \ptr is the pointer to the  \
	 * data, \len is the number of items in the vector and \cap is the     \
	 * capacity, i.e. the number of items it can store before reallocating.\
	 */                                                                    \
	typedef struct {                                                       \
		T *ptr;                                                        \
		size_t len;                                                    \
		size_t cap;                                                    \
	} type_name;                                                           \
                                                                               \
	type_name prefix##_init (void);                                        \
	type_name prefix##_init_with_cap (size_t cap);                         \
	type_name prefix##_from (const T *ptr, size_t len);                    \
	type_name prefix##_from_allocated (T *ptr, size_t len, size_t cap);    \
	type_name prefix##_repeat (T item, size_t n);                          \
	type_name prefix##_repeat_slice (const T *slice, size_t len, size_t rep);\
	void prefix##_deinit (type_name *vec);                                 \
	void prefix##_grow (type_name *vec, size_t cap);                       \
	void prefix##_clear (type_name *vec);                                  \
	void prefix##_trunc (type_name *vec, size_t n);                        \
	type_name prefix##_clone (type_name *vec);                             \
	void prefix##_append (type_name *vec, T item);                         \
	void prefix##_append_slice (type_name *vec, const T *slice, size_t slice_len); \
	void prefix##_pop (type_name *vec, T **item_out);                      \
	size_t prefix##_pop_slice (type_name *vec, T **slice_out, size_t max_len); \
	const T *prefix##_find (const type_name *vec, T item);                 \
	ssize_t prefix##_find_idx (const type_name *vec, T item);              \
	const T *prefix##_find_slice (const type_name *vec, const T *slice, size_t len); \
	ssize_t prefix##_find_slice_idx (const type_name *vec, const T *slice, size_t len); \
	bool prefix##_contains (const type_name *vec, T item);                 \
	bool prefix##_contains_slice (const type_name *vec, const T *slice, size_t len); \
	const T *prefix##_get (const type_name *vec, size_t idx);              \
	const T *prefix##_get_back (const type_name *vec, size_t idx);         \
	const T *prefix##_get_first (const type_name *vec);                    \
	const T *prefix##_get_last (const type_name *vec);                     \
	bool prefix##_set (const type_name *vec, size_t idx, T item);          \
	bool prefix##_set_slice (const type_name *vec, size_t idx, const T *slice, size_t len); \
	void prefix##_insert (type_name *vec, size_t idx, T item);             \
	void prefix##_insert_slice (type_name *restrict vec, size_t idx, const T *restrict slice, size_t len); \
	size_t prefix##_remove (type_name *vec, size_t idx, size_t count);     \
	size_t prefix##_count (const type_name *vec, T item);                  \
	size_t prefix##_count_slice (const type_name *vec, const T *slice, size_t len); \
	size_t prefix##_count_slice_overlap (const type_name *vec, const T *slice, size_t len); \
	void prefix##_reverse (type_name *vec);                                \
	void prefix##_sort (type_name *vec, int (* comp) (const T *, const T *)); \
	const T *prefix##_bsearch (const type_name *vec, const T *key, int (* comp) (const T *, const T *)); \
                                                                               \
	__e_vec_ensure_user_has_to_use_semicolon ()

/* implementation *************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ < 202000L || defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
# define e_priv_stdc_bit_ceil(x) (size_t) e_priv_stdc_bit_ceil_u64 ((uint64_t) x)
static uint64_t
e_priv_stdc_bit_ceil_u64 (uint64_t x)
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x |= x >> 32;
	x++;
	return x;
}
#else /* __STDC_VERSION__ < 202000L || defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
# include <stdbit.h>
# define e_priv_stdc_bit_ceil(x) stdc_bit_ceil (x)
#endif /* __STDC_VERSION__ < 202000L || defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */

/**
 * Implement the functions required for a vector of type \T with type \type_name
 * and function prefix \prefix. This goes into a `.c` file and requires there to
 * be a declaration somewhere in a header file with `E_VEC_DECL`.
 */
#define E_VEC_IMPL(T, type_name, prefix)                                       \
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
			.ptr = NULL,                                           \
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
	prefix##_init_with_cap (size_t cap)                                    \
	{                                                                      \
		T *ptr;                                                        \
		                                                               \
		ptr = E_CONFIG_MALLOC_FUNC (sizeof (T) * cap);                 \
		if (!ptr) {                                                    \
			fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n",\
			         sizeof (T) * cap);                            \
			exit (EXIT_FAILURE);                                   \
		}                                                              \
                                                                               \
		return prefix##_from_allocated (ptr, 0, cap);                  \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Create a vector from a region of memory pointed to by \ptr with     \
	 * length \len. If \ptr is `nullptr`, an empty vector is initialized   \
	 * regardless of \len. The memory from \ptr is copied into a newly     \
	 * allocated buffer.                                                   \
	 */                                                                    \
	type_name                                                              \
	prefix##_from (const T *ptr, size_t len)                               \
	{                                                                      \
		type_name ret;                                                 \
		                                                               \
		ret = prefix##_init_with_cap (len);                            \
		memcpy (ret.ptr, ptr, sizeof (T) * len);                       \
		ret.len = len;                                                 \
		return ret;                                                    \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Create a vector from a pointer \ptr, a length \len and the capacity \
	 * \cap. \ptr must be allocated on the heap using `e_alloc` and it     \
	 * must be capable of storing \cap items. \len is the number of items  \
	 * actually stored in \ptr.                                            \
	 */                                                                    \
	type_name                                                              \
	prefix##_from_allocated (T *ptr, size_t len, size_t cap)               \
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
	prefix##_repeat (T item, size_t n)                                     \
	{                                                                      \
		T *ptr;                                                        \
		size_t i;                                                      \
                                                                               \
		if (n == 0) return prefix##_init ();                           \
                                                                               \
		ptr = E_CONFIG_MALLOC_FUNC (sizeof (T) * n);                   \
		if (!ptr) {                                                    \
			fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n",\
			         sizeof (T) * n);                              \
			exit (EXIT_FAILURE);                                   \
		}                                                              \
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
	prefix##_repeat_slice (const T *slice, size_t len, size_t rep)         \
	{                                                                      \
		T *ptr;                                                        \
		size_t i;                                                      \
                                                                               \
		if (!slice || len == 0 || rep == 0) return prefix##_init ();   \
                                                                               \
		ptr = E_CONFIG_MALLOC_FUNC (sizeof (T) * len * rep);           \
		if (!ptr) {                                                    \
			fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n",\
			         sizeof (T) * len * rep);                      \
			exit (EXIT_FAILURE);                                   \
		}                                                              \
		for (i = 0; i < rep; i++) {                                    \
			memcpy (&ptr[i * len], slice, sizeof (T) * len);       \
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
			E_CONFIG_FREE_FUNC (vec->ptr);                         \
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
	prefix##_grow (type_name *vec, size_t cap)                             \
	{                                                                      \
		if (!vec) return;                                              \
		if (cap <= vec->cap) return;                                   \
		vec->cap = e_priv_stdc_bit_ceil (cap);                         \
		vec->ptr = E_CONFIG_REALLOC_FUNC (vec->ptr,                    \
		                                  sizeof (T) * vec->cap);      \
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
	prefix##_trunc (type_name *vec, size_t n)                              \
	{                                                                      \
		if (!vec) return;                                              \
		if (vec->len >= n) return;                                     \
		vec->len = n;                                                  \
		vec->cap = e_priv_stdc_bit_ceil (n);                           \
		vec->ptr = E_CONFIG_REALLOC_FUNC (vec->ptr, sizeof (T) * n);   \
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
		ptr = E_CONFIG_MALLOC_FUNC (sizeof (T) * vec->cap);            \
		if (!ptr) {                                                    \
			fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n",\
			         sizeof (T) * vec->cap);                       \
			exit (EXIT_FAILURE);                                   \
		}                                                              \
		memcpy (ptr, vec->ptr, sizeof (T) * vec->len);                 \
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
	prefix##_append_slice (type_name *vec, const T *slice,                 \
	                       size_t slice_len)                               \
	{                                                                      \
		if (!vec) return;                                              \
		if (vec->len + slice_len > vec->cap) {                         \
			prefix##_grow (vec, vec->len + slice_len);             \
		}                                                              \
		memcpy (&vec->ptr[vec->len], slice, sizeof (T) * slice_len);   \
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
			if (item_out) *item_out = NULL;                        \
			return;                                                \
		}                                                              \
                                                                               \
		if (vec->len > 0) {                                            \
			vec->len -= 1;                                         \
			if (item_out) *item_out = &vec->ptr[vec->len];         \
		} else {                                                       \
			if (item_out) *item_out = NULL;                        \
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
	size_t                                                                 \
	prefix##_pop_slice (type_name *vec, T **slice_out, size_t max_len)     \
	{                                                                      \
		size_t slice_len;                                              \
                                                                               \
		if (!vec) {                                                    \
			if (slice_out) *slice_out = NULL;                      \
			return 0;                                              \
		}                                                              \
                                                                               \
		if (vec->len > 0) {                                            \
			slice_len = vec->len < max_len ? vec->len : max_len;   \
			vec->len -= slice_len;                                 \
			if (slice_out) *slice_out = &vec->ptr[vec->len];       \
			return slice_len;                                      \
		} else {                                                       \
			if (slice_out) *slice_out = NULL;                      \
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
		ssize_t idx;                                                   \
                                                                               \
		idx = prefix##_find_idx (vec, item);                           \
		if (idx < 0) return NULL;                                      \
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
	ssize_t                                                                \
	prefix##_find_idx (const type_name *vec, T item)                       \
	{                                                                      \
		size_t i;                                                      \
                                                                               \
		if (!vec) return -1;                                           \
		for (i = 0; i < vec->len; i++) {                               \
			if (!memcmp (&vec->ptr[i], &item, sizeof (T))) {       \
				return (ssize_t) i;                            \
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
	prefix##_find_slice (const type_name *vec, const T *slice, size_t len) \
	{                                                                      \
		ssize_t idx;                                                   \
                                                                               \
		idx = prefix##_find_slice_idx (vec, slice, len);               \
		if (idx < 0) return NULL;                                      \
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
	ssize_t                                                                \
	prefix##_find_slice_idx (const type_name *vec, const T *slice,         \
	                         size_t len)                                   \
	{                                                                      \
		size_t i;                                                      \
                                                                               \
		if (!vec) return -1;                                           \
		if (!slice || len == 0) return vec->len == 0 ? -1 : 0;         \
		for (i = 0; i < vec->len - len + 1; i++) {                     \
			if (!memcmp (&vec->ptr[i], slice, sizeof (T) * len)) { \
				return (ssize_t) i;                            \
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
	                         size_t len)                                   \
	{                                                                      \
		return prefix##_find_slice_idx (vec, slice, len) >= 0;         \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Get the item at index \idx of the vector \vec. Returns a pointer to \
	 * the item. If \vec is `nullptr` or \idx is out of range, `nullptr`   \
	 * is returned.                                                        \
	 */                                                                    \
	const T *                                                              \
	prefix##_get (const type_name *vec, size_t idx)                        \
	{                                                                      \
		if (!vec) return NULL;                                         \
		if (idx >= vec->len) return NULL;                              \
		return (const T *) &vec->ptr[idx];                             \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Get the item at index \idx of the vector \vec, starting at the back \
	 * of the vector. For instance, using `idx = 2` in a vector with 10    \
	 * 10 items will yield the 8th item. Returns a pointer to the item. If \
	 * \vec is `nullptr` or \idx is out of range, `nullptr` is returned.   \
	 */                                                                    \
	const T *                                                              \
	prefix##_get_back (const type_name *vec, size_t idx)                   \
	{                                                                      \
		if (!vec) return NULL;                                         \
		if (idx >= vec->len) return NULL;                              \
		return (const T *) &vec->ptr[vec->len - idx - 1];              \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Get the first item of the vector \vec. If \vec is `nullptr` or if   \
	 * the vector has zero length, `nullptr` is returned.                  \
	 */                                                                    \
	const T *                                                              \
	prefix##_get_first (const type_name *vec)                              \
	{                                                                      \
		if (!vec || vec->len == 0) return NULL;                        \
		return (const T *) &vec->ptr[0];                               \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Get the last item of the vector \vec. If \vec is `nullptr` or if    \
	 * the vector has zero length, `nullptr` is returned.                  \
	 */                                                                    \
	const T *                                                              \
	prefix##_get_last (const type_name *vec)                               \
	{                                                                      \
		if (!vec || vec->len == 0) return NULL;                        \
		return (const T *) &vec->ptr[vec->len - 1];                    \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Set the item at index \idx of the vector \vec to \item. When \idx   \
	 * is out of range, the item is not set. No reallocation is performed. \
	 * When the item was set, `true` is returned. When the item was not    \
	 * set or \vec is `nullptr`, `false` is returned.                      \
	 */                                                                    \
	bool                                                                   \
	prefix##_set (const type_name *vec, size_t idx, T item)                \
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
	prefix##_set_slice (const type_name *vec, size_t idx, const T *slice,  \
	                    size_t len)                                        \
	{                                                                      \
		if (!vec) return false;                                        \
		if (idx + len - 1 >= vec->len) return false;                   \
		memcpy (&vec->ptr[idx], slice, sizeof (T) * len);              \
		return true;                                                   \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Insert \item at the position \idx into the vector \vec. The rest of \
	 * the vector is shifted backwards. If \idx is out of range, the item  \
	 * is appended to the vector. The memory is reallocated if necessary.  \
	 */                                                                    \
	void                                                                   \
	prefix##_insert (type_name *vec, size_t idx, T item)                   \
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
	prefix##_insert_slice (type_name *restrict vec, size_t idx,            \
	                       const T *restrict slice, size_t len)            \
	{                                                                      \
		if (!vec || !slice || len == 0) return;                        \
		if (idx >= vec->len) {                                         \
			prefix##_append_slice (vec, slice, len);               \
			return;                                                \
		}                                                              \
		if (vec->len >= vec->cap) {                                    \
			prefix##_grow (vec, vec->cap + len);                   \
		}                                                              \
		memmove (vec->ptr + idx + len, vec->ptr + idx,                 \
			 sizeof (T) * (vec->len - idx));                       \
		memcpy (vec->ptr + idx, slice, sizeof (T) * len);              \
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
	size_t                                                                 \
	prefix##_remove (type_name *vec, size_t idx, size_t count)             \
	{                                                                      \
		if (!vec || idx >= vec->len || count == 0) return 0;           \
		if (idx + count == vec->len) {                                 \
			vec->len -= count;                                     \
			return count;                                          \
		}                                                              \
		count = idx + count > vec->len ? vec->len - idx : count;       \
		memmove (vec->ptr + idx, vec->ptr + idx + count,               \
		         sizeof (T) * count);                                  \
		vec->len -= count;                                             \
		return count;                                                  \
	}                                                                      \
                                                                               \
	/**                                                                    \
	 * Count the number of occurances of \item in the vector \vec. If \vec \
	 * is `nullptr`, 0 is returned. For comparing the items, `memcmp` is   \
	 * used.                                                               \
	 */                                                                    \
	size_t                                                                 \
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
	size_t                                                                 \
	prefix##_count_slice (const type_name *vec, const T *slice, size_t len)\
	{                                                                      \
		size_t i, count;                                               \
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
	size_t                                                                 \
	prefix##_count_slice_overlap (const type_name *vec, const T *slice,    \
	                              size_t len)                              \
	{                                                                      \
		size_t i, count;                                               \
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
	/**                                                                    \
	 * Reverse the contents of the vector \vec.                            \
	 */                                                                    \
	void                                                                   \
	prefix##_reverse (type_name *vec) {                                    \
		T *start, *end, tmp;                                           \
                                                                               \
		if (!vec) return;                                              \
		if (vec->len == 0) return;                                     \
		start = &vec->ptr[0];                                          \
		end = &vec->ptr[vec->len - 1];                                 \
		while (start < end) {                                          \
			tmp = *start;                                          \
			*start++ = *end;                                       \
			*end-- = tmp;                                          \
		}                                                              \
	}                                                                      \
	                                                                       \
	/**                                                                    \
	 * Sort the vector \vec using the comparison function \comp in         \
	 * ascending order. \comp should return a negative integer when the    \
	 * value of the first argument is less than the value of the second    \
	 * argument, and a positive integer when the first argument is greater \
	 * than the second. If both arguments are equal, it should return 0.   \
	 * Internally, the `qsort` function of the standard library is used.   \
	 */                                                                    \
	void                                                                   \
	prefix##_sort (type_name *vec, int (* comp) (const T *, const T *))    \
	{                                                                      \
		if (!vec || !comp) return;                                     \
		qsort (vec->ptr, vec->len, sizeof (T),                         \
		       (int (*) (const void *, const void *)) comp);           \
	}                                                                      \
	                                                                       \
	/**                                                                    \
	 * Perform a binary search on the vector \vec for the key \key using   \
	 * the comparison function \comp. \vec MUST be sorted in ascending     \
	 * order as specified by \comp in order for this algorithm to work.    \
	 * \comp should return a negative integer when the value of the first  \
	 * argument is less than the value of the second argument, and a       \
	 * positive integer when the first argument is greater than the        \
	 * second. If both arguments are equal, it should return 0. Returns a  \
	 * pointer to the element if it was found, or `nullptr` if it was not  \
	 * found. Internally, the `bsearch` function of the standard library   \
	 * is used.                                                            \
	 */                                                                    \
	const T *                                                              \
	prefix##_bsearch (const type_name *vec, const T *key,                  \
	                  int (* comp) (const T *, const T *))                 \
	{                                                                      \
		if (!vec || !key || !comp) return NULL;                        \
		return bsearch (key, vec->ptr, vec->len, sizeof (T),           \
		                (int (*) (const void *, const void *)) comp);  \
	}                                                                      \
                                                                               \
	__e_vec_ensure_user_has_to_use_semicolon ()

#endif /* _EMPOWER_VEC_H_ */
