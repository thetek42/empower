/*! e_vec *********************************************************************
 *
 * This module provides "type-generic" vectors / dynamic arrays. The vectors
 * provided by this module allocate their data using `e_alloc`, i.e. `malloc`.
 * Like in `e_alloc`, when an allocation fails, the programme is terminated.
 *
 * To use this module, define the `E_VEC_TYPE`, `E_VEC_NAME` and `E_VEC_PREFIX`
 * macros and include the `e_vec.h` file for every type you want a vector. This
 * will create the type definition and function declarations for a vector of
 * items of type `E_VEC_TYPE`. The vector struct will be named `E_VEC_NAME` and
 * the vector functions will be prefixed with `E_VEC_PREFIX`. When `E_VEC_IMPL`
 * is defined, the implementations of the vector functions are generated.
 *  | #define E_VEC_TYPE int
 *  | #define E_VEC_NAME Vec_Int
 *  | #define E_VEC_PREFIX vec_int
 *  | #define E_VEC_IMPL
 *  | #include <e_vec.h>
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

#ifndef E_VEC_TYPE
# error "E_VEC_TYPE not defined"
#endif /* E_VEC_TYPE */
#ifndef E_VEC_NAME
# error "E_VEC_NAME not defined"
#endif /* E_VEC_NAME */
#ifndef E_VEC_PREFIX
# error "E_VEC_PREFIX not defined"
#endif /* E_VEC_PREFIX */

#define __E_VEC_CONCAT_HELPER(a, b) a##b
#define __E_VEC_CONCAT(a, b) __E_VEC_CONCAT_HELPER (a, b)
#define __E_VEC_FUNC(suffix) __E_VEC_CONCAT (E_VEC_PREFIX, __E_VEC_CONCAT (_, suffix))

/**
 * The type of the vector / dynamic array.ptr is the pointer to the data,len is
 * the number of items in the vector and \cap is the capacity, i.e. the number
 * of items it can store before reallocating.
 */
typedef struct {
	E_VEC_TYPE *ptr;
	size_t len;
	size_t cap;
} E_VEC_NAME;

E_VEC_NAME __E_VEC_FUNC (init) (void);
E_VEC_NAME __E_VEC_FUNC (init_with_cap) (size_t cap);
E_VEC_NAME __E_VEC_FUNC (from) (const E_VEC_TYPE *ptr, size_t len);
E_VEC_NAME __E_VEC_FUNC (from_allocated) (E_VEC_TYPE *ptr, size_t len, size_t cap);
E_VEC_NAME __E_VEC_FUNC (repeat) (E_VEC_TYPE item, size_t n);
E_VEC_NAME __E_VEC_FUNC (repeat_slice) (const E_VEC_TYPE *slice, size_t len, size_t rep);
void __E_VEC_FUNC (deinit) (E_VEC_NAME *vec);
void __E_VEC_FUNC (grow) (E_VEC_NAME *vec, size_t cap);
void __E_VEC_FUNC (clear) (E_VEC_NAME *vec);
void __E_VEC_FUNC (trunc) (E_VEC_NAME *vec, size_t n);
E_VEC_NAME __E_VEC_FUNC (clone) (E_VEC_NAME *vec);
void __E_VEC_FUNC (append) (E_VEC_NAME *vec, E_VEC_TYPE item);
void __E_VEC_FUNC (append_slice) (E_VEC_NAME *vec, const E_VEC_TYPE *slice, size_t slice_len);
void __E_VEC_FUNC (pop) (E_VEC_NAME *vec, E_VEC_TYPE **item_out);
size_t __E_VEC_FUNC (pop_slice) (E_VEC_NAME *vec, E_VEC_TYPE **slice_out, size_t max_len);
const E_VEC_TYPE *__E_VEC_FUNC (find) (const E_VEC_NAME *vec, E_VEC_TYPE item);
ssize_t __E_VEC_FUNC (find_idx) (const E_VEC_NAME *vec, E_VEC_TYPE item);
const E_VEC_TYPE *__E_VEC_FUNC (find_slice) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice, size_t len);
ssize_t __E_VEC_FUNC (find_slice_idx) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice, size_t len);
bool __E_VEC_FUNC (contains) (const E_VEC_NAME *vec, E_VEC_TYPE item);
bool __E_VEC_FUNC (contains_slice) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice, size_t len);
const E_VEC_TYPE *__E_VEC_FUNC (get) (const E_VEC_NAME *vec, size_t idx);
const E_VEC_TYPE *__E_VEC_FUNC (get_back) (const E_VEC_NAME *vec, size_t idx);
const E_VEC_TYPE *__E_VEC_FUNC (get_first) (const E_VEC_NAME *vec);
const E_VEC_TYPE *__E_VEC_FUNC (get_last) (const E_VEC_NAME *vec);
bool __E_VEC_FUNC (set) (const E_VEC_NAME *vec, size_t idx, E_VEC_TYPE item);
bool __E_VEC_FUNC (set_slice) (const E_VEC_NAME *vec, size_t idx, const E_VEC_TYPE *slice, size_t len);
void __E_VEC_FUNC (insert) (E_VEC_NAME *vec, size_t idx, E_VEC_TYPE item);
void __E_VEC_FUNC (insert_slice) (E_VEC_NAME *restrict vec, size_t idx, const E_VEC_TYPE *restrict slice, size_t len);
size_t __E_VEC_FUNC (remove) (E_VEC_NAME *vec, size_t idx, size_t count);
size_t __E_VEC_FUNC (count) (const E_VEC_NAME *vec, E_VEC_TYPE item);
size_t __E_VEC_FUNC (count_slice) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice, size_t len);
size_t __E_VEC_FUNC (count_slice_overlap) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice, size_t len);
void __E_VEC_FUNC (reverse) (E_VEC_NAME *vec);
void __E_VEC_FUNC (sort) (E_VEC_NAME *vec, int (* comp) (const E_VEC_TYPE *, const E_VEC_TYPE *));
const E_VEC_TYPE *__E_VEC_FUNC (bsearch) (const E_VEC_NAME *vec, const E_VEC_TYPE *key, int (* comp) (const E_VEC_TYPE *, const E_VEC_TYPE *));

/* implementation *************************************************************/

#ifdef E_VEC_IMPL

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
 * Initialise a vector with capacity 0. This function does not perform an
 * allocation. The vector must be freed by the user using `e_vec_deinit`.
 */
E_VEC_NAME
__E_VEC_FUNC (init) (void)
{
	return (E_VEC_NAME) {
		.ptr = NULL,
		.len = 0,
		.cap = 0,
	};
}

/**
 * Initialise a vector with a given capacity. This function allocates enough
 * memory to store \cap items. The vector must be freed by the user using
 * `e_vec_deinit`.
 */
E_VEC_NAME
__E_VEC_FUNC (init_with_cap) (size_t cap)
{
	E_VEC_TYPE *ptr;
	size_t alloc;

	alloc = sizeof (E_VEC_TYPE) * cap;
	ptr = E_CONFIG_MALLOC_FUNC (alloc);
	if (!ptr) {
		fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n", alloc);
		exit (EXIT_FAILURE);
	}

	return __E_VEC_FUNC (from_allocated) (ptr, 0, cap);
}

/**
 * Create a vector from a region of memory pointed to by \ptr with length \len.
 * If \ptr is `nullptr`, an empty vector is initialized regardless of \len. The
 * memory from \ptr is copied into a newly allocated buffer.
 */
E_VEC_NAME
__E_VEC_FUNC (from) (const E_VEC_TYPE *ptr, size_t len)
{
	E_VEC_NAME ret;

	ret = __E_VEC_FUNC (init_with_cap) (len);
	memcpy (ret.ptr, ptr, sizeof (E_VEC_TYPE) * len);
	ret.len = len;
	return ret;
}

/**
 * Create a vector from a pointer \ptr, a length \len and the capacity \cap.
 * \ptr must be allocated on the heap using `e_alloc` and it must be capable of
 * storing \cap items. \len is the number of items actually stored in \ptr.
 */
E_VEC_NAME
__E_VEC_FUNC (from_allocated) (E_VEC_TYPE *ptr, size_t len, size_t cap)
{
	return (E_VEC_NAME) {
		.ptr = ptr,
		.len = len,
		.cap = cap,
	};
}

/**
 * Initialise a vector with \n repeated items with value \item.
 */
E_VEC_NAME
__E_VEC_FUNC (repeat) (E_VEC_TYPE item, size_t n)
{
	E_VEC_TYPE *ptr;
	size_t i, alloc;

	if (n == 0) return __E_VEC_FUNC (init) ();

	alloc = sizeof (E_VEC_TYPE) * n;
	ptr = E_CONFIG_MALLOC_FUNC (alloc);
	if (!ptr) {
		fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n", alloc);
		exit (EXIT_FAILURE);
	}
	for (i = 0; i < n; i++) {
		ptr[i] = item;
	}

	return __E_VEC_FUNC (from_allocated) (ptr, n, n);
}

/**
 * Initialise a vector with a slice \slice of length \len repeated \rep times.
 * If \slice is `nullptr`, 0 repetitions are put into the vector, regardless of
 * \len.
 */
E_VEC_NAME
__E_VEC_FUNC (repeat_slice) (const E_VEC_TYPE *slice, size_t len, size_t rep)
{
	E_VEC_TYPE *ptr;
	size_t i, alloc;

	if (!slice || len == 0 || rep == 0) return __E_VEC_FUNC (init) ();

	alloc = sizeof (E_VEC_TYPE) * len * rep;
	ptr = E_CONFIG_MALLOC_FUNC (alloc);
	if (!ptr) {
		fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n", alloc);
		exit (EXIT_FAILURE);
	}
	for (i = 0; i < rep; i++) {
		memcpy (&ptr[i * len], slice, sizeof (E_VEC_TYPE) * len);
	}

	return __E_VEC_FUNC (from_allocated) (ptr, len * rep, len * rep);
}

/**
 * Deinitialise a vector and free the memory occupied by it. If the vector is
 * used for storing allocated pointers to data, the user must free this memory,
 * as it is not done here.
 */
void
__E_VEC_FUNC (deinit) (E_VEC_NAME *vec)
{
	if (vec) {
		E_CONFIG_FREE_FUNC (vec->ptr);
	}
}

/**
 * Grow the memory of a vector \vec to at least \cap items. If \cap is lower
 * than the current capacity of the vector, no action will be performed.
 * Internally, it rounds up \cap to the next power of two and reallocates the
 * memory to the new capacity.
 */
void
__E_VEC_FUNC (grow) (E_VEC_NAME *vec, size_t cap)
{
	E_VEC_TYPE *ptr;
	size_t alloc;

	if (!vec) return;
	if (cap <= vec->cap) return;

	vec->cap = e_priv_stdc_bit_ceil (cap);
	alloc = sizeof (E_VEC_TYPE) * vec->cap;
	ptr = E_CONFIG_REALLOC_FUNC (vec->ptr, alloc);
	if (!ptr) {
		fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n", alloc);
		exit (EXIT_FAILURE);
	}
	vec->ptr = ptr;
}

/**
 * Clear all items from a vector \vec. The memory is freed. This is the same as
 * calling `e_vec_deinit` followed by `e_vec_init`.
 */
void
__E_VEC_FUNC (clear) (E_VEC_NAME *vec)
{
	if (!vec) return;
	__E_VEC_FUNC (deinit) (vec);
	*vec = __E_VEC_FUNC (init) ();
}

/**
 * Truncate a vector \vec to \n items. The memory is reallocated (i.e. shrank)
 * to fit the new number of items. If \n is larger than the length of the
 * vector, no action is performed.
 */
void
__E_VEC_FUNC (trunc) (E_VEC_NAME *vec, size_t n)
{
	E_VEC_TYPE *ptr;
	size_t alloc;

	if (!vec) return;
	if (vec->len >= n) return;

	vec->len = n;
	vec->cap = e_priv_stdc_bit_ceil (n);
	alloc = sizeof (E_VEC_TYPE) * n;
	ptr = E_CONFIG_REALLOC_FUNC (vec->ptr, alloc);
	if (!ptr) {
		fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n", alloc);
		exit (EXIT_FAILURE);
	}
	vec->ptr = ptr;
}

/**
 * Clone a vector. This function allocates memory which must be freed by the
 * user using \e_vec_deinit. If \vec is `nullptr`, an empty vector is returned.
 */
E_VEC_NAME
__E_VEC_FUNC (clone) (E_VEC_NAME *vec)
{
	E_VEC_TYPE *ptr;
	size_t alloc;

	if (!vec) return __E_VEC_FUNC (init) ();

	alloc = sizeof (E_VEC_TYPE) * vec->cap;
	ptr = E_CONFIG_MALLOC_FUNC (alloc);
	if (!ptr) {
		fprintf (stderr, "[e_vec] failed to alloc %zu bytes\n", alloc);
		exit (EXIT_FAILURE);
	}
	memcpy (ptr, vec->ptr, sizeof (E_VEC_TYPE) * vec->len);

	return (E_VEC_NAME) {
		.ptr = ptr,
		.cap = vec->cap,
		.len = vec->len,
	};
}

/**
 * Add the item \item to the end of the vector \vec. If the vector does not have
 * enough capacity, its data will be reallocated to accommodate for the new
 * item.
 */
void
__E_VEC_FUNC (append) (E_VEC_NAME *vec, E_VEC_TYPE item)
{
	__E_VEC_FUNC (append_slice) (vec, &item, 1);
}

/**
 * Adds \slice_len items from the memory location \slice to the end of the
 * vector \vec. If the vector does not have enough capacity, its data will be
 * reallocated to accommodate for the new item.
 */
void
__E_VEC_FUNC (append_slice) (E_VEC_NAME *vec, const E_VEC_TYPE *slice,
                             size_t slice_len)
{
	if (!vec) return;
	if (vec->len + slice_len > vec->cap) {
		__E_VEC_FUNC (grow) (vec, vec->len + slice_len);
	}
	memcpy (&vec->ptr[vec->len], slice, sizeof (E_VEC_TYPE) * slice_len);
	vec->len += slice_len;
}

/**
 * Pop the last item from the vector \vec. The item will be stored in \item_out.
 * If \vec is empty or `nullptr`, \item_out will be set to `nullptr`. The length
 * field of the vector will be decremented. If \item_out is `nullptr`, the item
 * will be popped and the length field will be updated, but \item_out will not
 * be filled. The memory of \item_out will remain valid until the vector buffer
 * is reallocated. If the vector is used for storing allocated pointers to data,
 * the user must free this memory, as it is not done here.
 */
void
__E_VEC_FUNC (pop) (E_VEC_NAME *vec, E_VEC_TYPE **item_out)
{
	if (!vec) {
		if (item_out) *item_out = NULL;
		return;
	}

	if (vec->len > 0) {
		vec->len -= 1;
		if (item_out) *item_out = &vec->ptr[vec->len];
	} else {
		if (item_out) *item_out = NULL;
	}
}

/**
 * Pop the last items from the vector \vec. \max_len represents the maxmimum
 * number of items that should be popped. The number of items which were
 * actually popped is returned. \slice_out will be set to point to the first of
 * the popped items. If \vec is empty or `nullptr`, \slice_out will be set to
 * `nullptr` and 0 will be returned. The length field of the vector will be
 * decremented by the number of popped items. If \slice_out is `nullptr`, the
 * items will be popped, the length field will be updated and the number of
 * popped items will be returned, but \slice_out will not be filled. The memory
 * of \slice_out will remain valid until the vector buffer is reallocated. If
 * the vector is used for storing allocated pointers to data, the user must free
 * this memory, as it is not done here.
 */
size_t
__E_VEC_FUNC (pop_slice) (E_VEC_NAME *vec, E_VEC_TYPE **slice_out,
                          size_t max_len)
{
	size_t slice_len;

	if (!vec) {
		if (slice_out) *slice_out = NULL;
		return 0;
	}

	if (vec->len > 0) {
		slice_len = vec->len < max_len ? vec->len : max_len;
		vec->len -= slice_len;
		if (slice_out) *slice_out = &vec->ptr[vec->len];
		return slice_len;
	} else {
		if (slice_out) *slice_out = NULL;
		return 0;
	}
}

/**
 * Find the position of \item in the vector \vec. In order to find the item,
 * `memcmp` between \item and the items in the vector is used. Returns a pointer
 * to the item. If you want to get the index instead of the pointer, use
 * `e_vec_find_idx`. If \item is not found or \vec is `nullptr`, `nullptr` is
 * returned.
 */
const E_VEC_TYPE *
__E_VEC_FUNC (find) (const E_VEC_NAME *vec, E_VEC_TYPE item)
{
	ssize_t idx;

	idx = __E_VEC_FUNC (find_idx) (vec, item);
	if (idx < 0) return NULL;
	return (const E_VEC_TYPE *) &vec->ptr[idx];
}

/**
 * Find the position of \item in the vector \vec. In order to find the item,
 * `memcmp` between \item and the items in the vector is used. Returns the index
 * of the item. If you want to get the pointer instead of the index, use
 * `e_vec_find`. If \item is not found or \vec is `nullptr`, -1 is returned.
 */
ssize_t
__E_VEC_FUNC (find_idx) (const E_VEC_NAME *vec, E_VEC_TYPE item)
{
	size_t i;

	if (!vec) return -1;
	for (i = 0; i < vec->len; i++) {
		if (!memcmp (&vec->ptr[i], &item, sizeof (E_VEC_TYPE))) {
			return (ssize_t) i;
		}
	}
	return -1;
}

/**
 * Find the position of a slice of memory \slice with length \len in the vector
 * \vec. In order to find the items, `memcmp` between \slice and the items in
 * the vector is used. Returns a pointer to the first item of the matched slice.
 * If you want to get the index instead of the pointer, use
 * `e_vec_find_slice_idx`. If \item is not found or \vec is `nullptr`, `nullptr`
 * is returned. If \slice is `nullptr` or \len is 0 and \vec is has at least one
 * element, a pointer to that element is returned.
 */
const E_VEC_TYPE *
__E_VEC_FUNC (find_slice) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice,
                           size_t len)
{
	ssize_t idx;

	idx = __E_VEC_FUNC (find_slice_idx) (vec, slice, len);
	if (idx < 0) return NULL;
	return (const E_VEC_TYPE *) &vec->ptr[idx];
}

/**
 * Find the position of a slice of memory \slice with length \len in the vector
 * \vec. In order to find the items, `memcmp` between \slice and the items in
 * the vector is used. Returns the index of the first item of the matched slice.
 * If you want to get the pointer instead of the index of the first item, use
 * `e_vec_find_slice`. If \item is not found or \vec is `nullptr`, `nullptr` is
 * returned. If \slice is `nullptr` or \len is 0 and \vec has at least one
 * element, the first element's index (i.e. 0) is returned, -1 otherwise.
 */
ssize_t
__E_VEC_FUNC (find_slice_idx) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice,
                               size_t len)
{
	size_t i;

	if (!vec) return -1;
	if (!slice || len == 0) return vec->len == 0 ? -1 : 0;
	for (i = 0; i < vec->len - len + 1; i++) {
		if (!memcmp (&vec->ptr[i], slice, sizeof (E_VEC_TYPE) * len)) {
			return (ssize_t) i;
		}
	}
	return -1;
}

/**
 * Check if the vector \vec contains the item \item. For comparing the items in
 * \vec with \item, `memcmp` is used. Returns `true` if the item is found,
 * `false` otherwise. If \vec is `nullptr`, `false` is returned.
 */
bool
__E_VEC_FUNC (contains) (const E_VEC_NAME *vec, E_VEC_TYPE item)
{
	return __E_VEC_FUNC (find_idx) (vec, item) >= 0;
}

/**
 * Check if the vector \vec contains the slice \slice of length \len. For
 * comparing the items in \vec with \slice, `memcmp` is used. Returns `true` if
 * the slice is found, `false` otherwise. If \vec is `nullptr`, `false` is
 * returned. If \slice is `nullptr` or \len is 0, `true` is returned.
 */
bool
__E_VEC_FUNC (contains_slice) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice,
                               size_t len)
{
	return __E_VEC_FUNC (find_slice_idx) (vec, slice, len) >= 0;
}

/**
 * Get the item at index \idx of the vector \vec. Returns a pointer to the item.
 * If \vec is `nullptr` or \idx is out of range, `nullptr` is returned.
 */
const E_VEC_TYPE *
__E_VEC_FUNC (get) (const E_VEC_NAME *vec, size_t idx)
{
	if (!vec) return NULL;
	if (idx >= vec->len) return NULL;
	return (const E_VEC_TYPE *) &vec->ptr[idx];
}

/**
 * Get the item at index \idx of the vector \vec, starting at the back of the
 * vector. For instance, using `idx = 2` in a vector with 10 10 items will yield
 * the 8th item. Returns a pointer to the item. If \vec is `nullptr` or \idx is
 * out of range, `nullptr` is returned.
 */
const E_VEC_TYPE *
__E_VEC_FUNC (get_back) (const E_VEC_NAME *vec, size_t idx)
{
	if (!vec) return NULL;
	if (idx >= vec->len) return NULL;
	return (const E_VEC_TYPE *) &vec->ptr[vec->len - idx - 1];
}

/**
 * Get the first item of the vector \vec. If \vec is `nullptr` or if the vector
 * has zero length, `nullptr` is returned.
 */
const E_VEC_TYPE *
__E_VEC_FUNC (get_first) (const E_VEC_NAME *vec)
{
	if (!vec || vec->len == 0) return NULL;
	return (const E_VEC_TYPE *) &vec->ptr[0];
}

/**
 * Get the last item of the vector \vec. If \vec is `nullptr` or if the vector
 * has zero length, `nullptr` is returned.
 */
const E_VEC_TYPE *
__E_VEC_FUNC (get_last) (const E_VEC_NAME *vec)
{
	if (!vec || vec->len == 0) return NULL;
	return (const E_VEC_TYPE *) &vec->ptr[vec->len - 1];
}

/**
 * Set the item at index \idx of the vector \vec to \item. When \idx is out of
 * range, the item is not set. No reallocation is performed. When the item was
 * set, `true` is returned. When the item was not set or \vec is `nullptr`,
 * `false` is returned.
 */
bool
__E_VEC_FUNC (set) (const E_VEC_NAME *vec, size_t idx, E_VEC_TYPE item)
{
	if (!vec) return false;
	if (idx >= vec->len) return false;
	vec->ptr[idx] = item;
	return true;
}

/**
 * Set the items beginning at index \idx of the vector \vec to the slice \slice
 * of length \len. When \idx is out of range, the items are not set. No
 * reallocation is performed. When the items were set, `true` is returned. When
 * the items were not set or \vec is `nullptr`, `false` is returned.
 */
bool
__E_VEC_FUNC (set_slice) (const E_VEC_NAME *vec, size_t idx,
                          const E_VEC_TYPE *slice, size_t len)
{
	if (!vec) return false;
	if (idx + len - 1 >= vec->len) return false;
	memcpy (&vec->ptr[idx], slice, sizeof (E_VEC_TYPE) * len);
	return true;
}

/**
 * Insert \item at the position \idx into the vector \vec. The rest of the
 * vector is shifted backwards. If \idx is out of range, the item is appended to
 * the vector. The memory is reallocated if necessary.
 */
void
__E_VEC_FUNC (insert) (E_VEC_NAME *vec, size_t idx, E_VEC_TYPE item)
{
	__E_VEC_FUNC (insert_slice) (vec, idx, &item, 1);
}

/**
 * Insert a slice \slice of \len items at position \idx into the vector \vec.
 * The rest of the vector is shifted backwards. If \idx is out of range, the
 * items are appended to the vector. The memory is reallocated if necessary.
 */
void
__E_VEC_FUNC (insert_slice) (E_VEC_NAME *restrict vec, size_t idx,
                             const E_VEC_TYPE *restrict slice, size_t len)
{
	size_t byte_count;

	if (!vec || !slice || len == 0) return;

	if (idx >= vec->len) {
		__E_VEC_FUNC (append_slice) (vec, slice, len);
		return;
	}

	if (vec->len >= vec->cap) {
		__E_VEC_FUNC (grow) (vec, vec->cap + len);
	}
	byte_count = sizeof (E_VEC_TYPE) * (vec->len - idx);
	memmove (vec->ptr + idx + len, vec->ptr + idx, byte_count);
	memcpy (vec->ptr + idx, slice, sizeof (E_VEC_TYPE) * len);
	vec->len += len;
}

/**
 * Remove \count items from the vector \vec starting at position \idx. No action
 * is performed for items at positions which would be out of bounds. When
 * removing memory in the middle of the vector, the memory is shifted
 * accordingly. Returns the number of removed elements.
 */
size_t
__E_VEC_FUNC (remove) (E_VEC_NAME *vec, size_t idx, size_t count)
{
	size_t byte_count;

	if (!vec || idx >= vec->len || count == 0) return 0;

	if (idx + count == vec->len) {
		vec->len -= count;
		return count;
	}

	count = idx + count > vec->len ? vec->len - idx : count;
	byte_count = sizeof (E_VEC_TYPE) * count;
	memmove (vec->ptr + idx, vec->ptr + idx + count, byte_count);
	vec->len -= count;
	return count;
}

/**
 * Count the number of occurances of \item in the vector \vec. If \vec is
 * `nullptr`, 0 is returned. For comparing the items, `memcmp` is used.
 */
size_t
__E_VEC_FUNC (count) (const E_VEC_NAME *vec, E_VEC_TYPE item)
{
	return __E_VEC_FUNC (count_slice_overlap) (vec, &item, 1);
}

/**
 * Count the number of occurances of a slice \slice of length \len in the vector
 * \vec. If \vec is `nullptr`, 0 is returned. For comparing the items, `memcmp`
 * is used. Overlap is not counted.
 */
size_t
__E_VEC_FUNC (count_slice) (const E_VEC_NAME *vec, const E_VEC_TYPE *slice,
                            size_t len)
{
	size_t i, count;

	if (!vec) return 0;
	count = 0;
	for (i = 0; i < vec->len; ) {
		if (!memcmp (&vec->ptr[i], slice, sizeof (E_VEC_TYPE) * len)) {
			count += 1;
			i += len;
		} else {
			i += 1;
		}
	}

	return count;
}

/**
 * Count the number of occurances of a slice \slice of length \len in the vector
 * \vec. If \vec is `nullptr`, 0 is returned. For comparing the items, `memcmp`
 * is used. Overlap is counted.
 */
size_t
__E_VEC_FUNC (count_slice_overlap) (const E_VEC_NAME *vec,
                                    const E_VEC_TYPE *slice, size_t len)
{
	size_t i, count;

	if (!vec) return 0;
	count = 0;
	for (i = 0; i < vec->len; i++) {
		if (!memcmp (&vec->ptr[i], slice, sizeof (E_VEC_TYPE) * len)) {
			count += 1;
		}
	}

	return count;
}

/**
 * Reverse the contents of the vector \vec.
 */
void
__E_VEC_FUNC (reverse) (E_VEC_NAME *vec) {
	E_VEC_TYPE *start, *end, tmp;

	if (!vec) return;
	if (vec->len == 0) return;
	start = &vec->ptr[0];
	end = &vec->ptr[vec->len - 1];
	while (start < end) {
		tmp = *start;
		*start++ = *end;
		*end-- = tmp;
	}
}

/**
 * Sort the vector \vec using the comparison function \comp in ascending order.
 * \comp should return a negative integer when the value of the first argument
 * is less than the value of the second argument, and a positive integer when
 * the first argument is greater than the second. If both arguments are equal,
 * it should return 0. Internally, the `qsort` function of the standard library
 * is used.
 */
void
__E_VEC_FUNC (sort) (E_VEC_NAME *vec,
                     int (* comp) (const E_VEC_TYPE *, const E_VEC_TYPE *))
{
	if (!vec || !comp) return;
	qsort (vec->ptr, vec->len, sizeof (E_VEC_TYPE),
	       (int (*) (const void *, const void *)) comp);
}

/**
 * Perform a binary search on the vector \vec for the key \key using the
 * comparison function \comp. \vec MUST be sorted in ascending order as
 * specified by \comp in order for this algorithm to work. \comp should return a
 * negative integer when the value of the first argument is less than the value
 * of the second argument, and a positive integer when the first argument is
 * greater than the second. If both arguments are equal, it should return 0.
 * Returns a pointer to the element if it was found, or `nullptr` if it was not
 * found. Internally, the `bsearch` function of the standard library is used.
 */
const E_VEC_TYPE *
__E_VEC_FUNC (bsearch) (const E_VEC_NAME *vec, const E_VEC_TYPE *key,
                        int (* comp) (const E_VEC_TYPE *, const E_VEC_TYPE *))
{
	if (!vec || !key || !comp) return NULL;
	return bsearch (key, vec->ptr, vec->len, sizeof (E_VEC_TYPE),
	                (int (*) (const void *, const void *)) comp);
}

#endif /* E_VEC_IMPL */

/* undefs *********************************************************************/

#undef __E_VEC_CONCAT
#undef __E_VEC_CONCAT_HELPER
#undef __E_VEC_FUNC
#undef E_VEC_TYPE
#undef E_VEC_NAME
#undef E_VEC_PREFIX
