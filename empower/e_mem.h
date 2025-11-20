#ifndef EMPOWER_MEM_H_
#define EMPOWER_MEM_H_

/******************************************************************************
 * 
 * This module provides miscellaneous functions for operating on memory.
 *
 * It provides various macro wrappers for the functions that require you to
 * provide the type of the arguments. This give extra "type safety" so that you
 * don't accidentally mess up. For exapmle, instead of:
 *   memcpy (dest, src, sizeof (int) * 42);
 *   memset (ptr, 0, sizeof (struct foo) * 64);
 * You can use:
 *   e_mem_copy (dest, src, int, 42);
 *   e_mem_zero (ptr, struct foo, 64);
 *
 * Configuration options:
 *   - E_CONFIG_FREESTANDING: Do not use functions from the standard library.
 *
 ******************************************************************************/

#include <stddef.h>
#if __STDC_VERSION__ >= 199901L
# include <stdint.h>
#endif

#if !defined (E_ALIGNOF)
# if __STDC_VERSION__ >= 202311L
#  define E_ALIGNOF(expr) alignof (expr)
# elif __STDC_VERSION__ >= 201112L
#  define E_ALIGNOF(expr) _Alignof (expr)
# elif defined (__GNUC__) || defined (__clang__) || defined (__TINYC__)
#  define E_ALIGNOF(expr) __alignof__ (expr)
# elif defined (_MSC_VER)
#  define E_ALIGNOF(expr) __alignof (expr)
# else
#  define E_ALIGNOF(type) ((char *) (&((struct { char c; type t; } *) 0)->t) - (char *) 0)
# endif
#endif

#define e_mem_copy(dest, src, T, n) memcpy ((dest), (src), sizeof (T) * (n))
#define e_mem_copy_size(dest, src, size) memcpy ((dest), (src), (size))
#define e_mem_move(dest, src, T, n) memmove ((dest), (src), sizeof (T) * (n))
#define e_mem_move_size(dest, src, size) memmove ((dest), (src), (size))
#define e_mem_zero(ptr, T, n) memset ((ptr), 0, sizeof (T) * (n))
#define e_mem_eq(a, b, T, n) e_mem_eq_size ((a), (b), sizeof (T) * (n))
#define e_mem_swap(a, b, T, n) e_mem_swap_size ((a), (b), sizeof (T) * (n))
#define e_mem_is_zero(ptr, T, n) e_mem_is_zero_size ((ptr), sizeof (T) * (n))
#define e_mem_clone(ptr, T, n) e_mem_clone_size ((ptr), sizeof (T) * (n))
#define e_mem_is_aligned_to_type(ptr, T) e_mem_is_aligned_to ((ptr), E_ALIGNOF (T))

int e_mem_eq_size (const void *a, const void *b, size_t n);
int e_mem_is_zero_size (const void *ptr, size_t n);
void e_mem_swap_size (void *a, void *b, size_t n);
void e_mem_reverse_bytes (void *ptr, size_t n);
int e_mem_is_aligned_to (const void *ptr, size_t align);

#ifndef E_CONFIG_FREESTANDING
char *e_mem_strdup (const char *s);
char *e_mem_strdup_n (const char *s, size_t n);
void *e_mem_clone_size (const void *ptr, size_t n);
#endif /* E_CONFIG_FREESTANDING */

#if __STDC_VERSION__ >= 199901L
uint16_t e_mem_read_u16_be (const uint8_t *mem);
uint16_t e_mem_read_u16_le (const uint8_t *mem);
uint32_t e_mem_read_u32_be (const uint8_t *mem);
uint32_t e_mem_read_u32_le (const uint8_t *mem);
uint64_t e_mem_read_u64_be (const uint8_t *mem);
uint64_t e_mem_read_u64_le (const uint8_t *mem);
void e_mem_write_u16_be (uint8_t *mem, uint16_t value);
void e_mem_write_u16_le (uint8_t *mem, uint16_t value);
void e_mem_write_u32_be (uint8_t *mem, uint32_t value);
void e_mem_write_u32_le (uint8_t *mem, uint32_t value);
void e_mem_write_u64_be (uint8_t *mem, uint64_t value);
void e_mem_write_u64_le (uint8_t *mem, uint64_t value);
#endif

/* implementation *************************************************************/

#ifdef E_MEM_IMPL

#ifndef E_CONFIG_FREESTANDING
# include <stdlib.h>
# include <string.h>
#endif /* E_CONFIG_FREESTANDING */

/**
 * Check if the memory region pointed to by `ptr` with size of `n` bytes is
 * zeroed. A non-zero value is returned if the memory is zeroed or `n` is 0.
 */
int
e_mem_is_zero_size (const void *ptr,
                    size_t n)
{
	const unsigned char *p;
	size_t i;

	p = ptr;
	for (i = 0; i < n; i++) {
		if (p[i] != 0) {
			return 0;
		}
	}

	return 1;
}

/**
 * Check if two memory regions `a` and `b` with a size of `n` bytes are equal.
 * When the memory of `a` and `b` is equal or if both `a` and `b` are NULL, a
 * non-zero value is returned.
 *
 * This function is essentially equivalent to `memcmp`, except that passing NULL
 * is allowed.
 */
int
e_mem_eq_size (const void *a,
               const void *b,
               size_t n)
{
	const unsigned char *ap = a;
	const unsigned char *bp = b;
	size_t i;

	if (a == b) return 1;
	if (a == NULL || b == NULL) return 0;
	for (i = 0; i < n; i++) {
		if (ap[i] != bp[i]) return 0;
	}
	return 1;
}

/**
 * Swaps `n` bytes of memory from `a` and `b`.
 */
void
e_mem_swap_size (void *a,
                 void *b,
                 size_t n)
{
	unsigned char *ap, *bp, tmp;
	size_t i;

	ap = a;
	bp = b;
	for (i = 0; i < n; i++) {
		tmp = ap[i];
		ap[i] = bp[i];
		bp[i] = tmp;
	}
}

/**
 * Reverse the bytes at the memory location `ptr`. `n` bytes will be reversed.
 */
void
e_mem_reverse_bytes (void *ptr, size_t n)
{
	unsigned char *start, *end, tmp;

	if (n == 0) return;
	start = ptr;
	end = start + n - 1;
	while (start < end) {
		tmp = *start;
		*start++ = *end;
		*end-- = tmp;
	}
}

/**
 * Checks if the pointer `ptr` is aligned to `align` bytes. If the pointer is
 * aligned, a non-zero value is returned.
 */
int
e_mem_is_aligned_to (const void *ptr, size_t align)
{
#if __STDC_VERSION__ >= 199901L
	return ((uintptr_t) ptr % align) == 0;
#else
	return ((size_t) ptr % align) == 0;
#endif
}

#ifndef E_CONFIG_FREESTANDING

/**
 * Duplicate a string. This is equivalent to `strdup`. The allocated memory must
 * be freed by the user using `free()`.
 */
char *
e_mem_strdup (const char *s)
{
	size_t len;
	char *res;

	if (!s) return NULL;
	len = strlen (s);
	res = malloc (sizeof (char) * (len + 1));
	if (res == NULL) return NULL;
	strncpy (res, s, len);
	res[len] = 0;

	return res;
}

/**
 * Duplicate at most `n` bytes of a string. This is equivalent to `strndup`. The
 * allocated memory must be freed by the user using `free()`.
 */
char *
e_mem_strdup_n (const char *s, size_t n)
{
	size_t len;
	char *res;

	if (!s) return NULL;
	len = strlen (s);
	len = len < n ? len : n;
	res = malloc (sizeof (char) * (len + 1));
	if (res == NULL) return NULL;
	strncpy (res, s, len);
	res[len] = 0;

	return res;
}

/**
 * Duplicate `n` bytes of memory from the pointer `ptr`. The allocated memory
 * must be freed by the user using `free()`. Undefined behaviour occurs if `ptr`
 * is NULL or `n` is 0.
 */
void *
e_mem_clone_size (const void *ptr, size_t n)
{
	void *ret;
	ret = malloc (n);
	if (ret == NULL) return NULL;
	memcpy (ret, ptr, n);
	return ret;
}

#endif /* E_CONFIG_FREESTANDING */

#if __STDC_VERSION__ >= 199901L

/**
 * Read a big-endian uint16_t from the memory `mem`.
 *
 * Example: { 0x12, 0x34 } => 0x1234
 */
uint16_t
e_mem_read_u16_be (const uint8_t *mem)
{
	return (uint16_t) ((((uint16_t) mem[0]) << 8) |
	                   (((uint16_t) mem[1]) << 0));
}

/**
 * Read a little-endian uint16_t from the memory `mem`.
 *
 * Example: { 0x12, 0x34 } => 0x3412
 */
uint16_t
e_mem_read_u16_le (const uint8_t *mem)
{
	return (uint16_t) ((((uint16_t) mem[1]) << 8) |
	                   (((uint16_t) mem[0]) << 0));
}

/**
 * Read a big-endian uint32_t from the memory `mem`.
 *
 * Example: { 0x12, 0x34, 0x56, 0x78 } => 0x12345678
 */
uint32_t
e_mem_read_u32_be (const uint8_t *mem)
{
	return (((uint32_t) mem[0]) << 24) |
	       (((uint32_t) mem[1]) << 16) |
	       (((uint32_t) mem[2]) <<  8) |
	       (((uint32_t) mem[3]) <<  0);
}

/**
 * Read a little-endian uint32_t from the memory `mem`.
 *
 * Example: { 0x12, 0x34, 0x56, 0x78 } => 0x78563412
 */
uint32_t
e_mem_read_u32_le (const uint8_t *mem)
{
	return (((uint32_t) mem[3]) << 24) |
	       (((uint32_t) mem[2]) << 16) |
	       (((uint32_t) mem[1]) <<  8) |
	       (((uint32_t) mem[0]) <<  0);
}

/**
 * Read a big-endian uint64_t from the memory `mem`.
 *
 * Example: { 0x12, 0x34, 0x56, ..., 0x34, 0x56 } => 0x1234567890123456
 */
uint64_t
e_mem_read_u64_be (const uint8_t *mem)
{
	return (((uint64_t) mem[0]) << 56) |
	       (((uint64_t) mem[1]) << 48) |
	       (((uint64_t) mem[2]) << 40) |
	       (((uint64_t) mem[3]) << 32) |
	       (((uint64_t) mem[4]) << 24) |
	       (((uint64_t) mem[5]) << 16) |
	       (((uint64_t) mem[6]) <<  8) |
	       (((uint64_t) mem[7]) <<  0);
}

/**
 * Read a little-endian uint64_t from the memory `mem`.
 *
 * Example: { 0x12, 0x34, 0x56, ..., 0x34, 0x56 } => 0x5634129078563412
 */
uint64_t
e_mem_read_u64_le (const uint8_t *mem)
{
	return (((uint64_t) mem[7]) << 56) |
	       (((uint64_t) mem[6]) << 48) |
	       (((uint64_t) mem[5]) << 40) |
	       (((uint64_t) mem[4]) << 32) |
	       (((uint64_t) mem[3]) << 24) |
	       (((uint64_t) mem[2]) << 16) |
	       (((uint64_t) mem[1]) <<  8) |
	       (((uint64_t) mem[0]) <<  0);
}

/**
 * Write a big-endian uint16_t `value` to the memory `mem`.
 *
 * Example: 0x1234 => { 0x12, 0x34 }
 */
void
e_mem_write_u16_be (uint8_t *mem, uint16_t value)
{
	mem[0] = (uint8_t) (value >> 8);
	mem[1] = (uint8_t) (value >> 0);
}

/**
 * Write a little-endian uint16_t `value` to the memory `mem`.
 *
 * Example: 0x3412 => { 0x12, 0x34 }
 */
void
e_mem_write_u16_le (uint8_t *mem, uint16_t value)
{
	mem[1] = (uint8_t) (value >> 8);
	mem[0] = (uint8_t) (value >> 0);
}

/**
 * Write a big-endian uint32_t `value` to the memory `mem`.
 *
 * Example: 0x12345678 => { 0x12, 0x34, 0x56, 0x78 }
 */
void
e_mem_write_u32_be (uint8_t *mem, uint32_t value)
{
	mem[0] = (uint8_t) (value >> 24);
	mem[1] = (uint8_t) (value >> 16);
	mem[2] = (uint8_t) (value >>  8);
	mem[3] = (uint8_t) (value >>  0);
}

/**
 * Write a little-endian uint32_t `value` to the memory `mem`.
 *
 * Example: 0x78563412 => { 0x12, 0x34, 0x56, 0x78 }
 */
void
e_mem_write_u32_le (uint8_t *mem, uint32_t value)
{
	mem[3] = (uint8_t) (value >> 24);
	mem[2] = (uint8_t) (value >> 16);
	mem[1] = (uint8_t) (value >>  8);
	mem[0] = (uint8_t) (value >>  0);
}

/**
 * Write a big-endian uint64_t `value` to the memory `mem`.
 *
 * Example: 0x1234567890123456 => { 0x12, 0x34, 0x56, ..., 0x34, 0x56 }
 */
void
e_mem_write_u64_be (uint8_t *mem, uint64_t value)
{
	mem[0] = (uint8_t) (value >> 56);
	mem[1] = (uint8_t) (value >> 48);
	mem[2] = (uint8_t) (value >> 40);
	mem[3] = (uint8_t) (value >> 32);
	mem[4] = (uint8_t) (value >> 24);
	mem[5] = (uint8_t) (value >> 16);
	mem[6] = (uint8_t) (value >>  8);
	mem[7] = (uint8_t) (value >>  0);
}

/**
 * Write a little-endian uint64_t `value` to the memory `mem`.
 *
 * Example: 0x5634129078563412 => { 0x12, 0x34, 0x56, ..., 0x34, 0x56 }
 */
void
e_mem_write_u64_le (uint8_t *mem, uint64_t value)
{
	mem[7] = (uint8_t) (value >> 56);
	mem[6] = (uint8_t) (value >> 48);
	mem[5] = (uint8_t) (value >> 40);
	mem[4] = (uint8_t) (value >> 32);
	mem[3] = (uint8_t) (value >> 24);
	mem[2] = (uint8_t) (value >> 16);
	mem[1] = (uint8_t) (value >>  8);
	mem[0] = (uint8_t) (value >>  0);
}

#endif /* __STDC_VERSION__ >= 199901L */

#endif /* E_MEM_IMPL */

#endif /* EMPOWER_MEM_H_ */
