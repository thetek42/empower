#ifndef _EMPOWER_MEM_H_
#define _EMPOWER_MEM_H_

/*! e_mem *********************************************************************
 * 
 * This module provides miscellaneous functions for operating on memory.
 *
 ******************************************************************************/

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* public interface ***********************************************************/

#define e_mem_copy(dest, src, T, n) memcpy ((dest), (src), sizeof (T) * (n))
#define e_mem_copy_size(dest, src, size) memcpy ((dest), (src), (size))
#define e_mem_move(dest, src, T, n) memmove ((dest), (src), sizeof (T) * (n))
#define e_mem_move_size(dest, src, size) memmove ((dest), (src), (size))
#define e_mem_eq(a, b, T, n) e_mem_eq_size ((a), (b), sizeof (T) * (n))
#define e_mem_swap(a, b, T, n) e_mem_swap_size ((a), (b), sizeof (T) * (n))
#define e_mem_is_zero(ptr, T, n) e_mem_is_zero_size ((ptr), sizeof (T) * (n))
#define e_mem_clone(ptr, T, n) e_mem_clone_size ((ptr), sizeof (T) * (n))
#define e_mem_is_aligned_to_type(ptr, T) e_mem_is_aligned_to ((ptr), alignof (T))

bool e_mem_is_aligned_to (const void *ptr, size_t align);
bool e_mem_eq_size (const void *a, const void *b, size_t n);
bool e_mem_is_zero_size (const void *ptr, size_t n);
void e_mem_swap_size (void *a, void *b, size_t n);
void e_mem_reverse_bytes (void *ptr, size_t len);
char *e_mem_strdup (const char *s);
char *e_mem_strdup_n (const char *s, size_t n);
void *e_mem_clone_size (const void *ptr, size_t n);
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

/* implementation *************************************************************/

#ifdef E_MEM_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Check if the memory region pointed to by \ptr with size of \n bytes is
 * zeroed. When \ptr is `nullptr` or \n is 0, `true` is returned.
 */
bool
e_mem_is_zero_size (const void *ptr, size_t n)
{
	const uint8_t *p;
	size_t i;

	if (!ptr) return true;
	p = ptr;
	for (i = 0; i < n; i++) {
		if (p[i] != 0) {
			return false;
		}
	}

	return true;
}

/**
 * Check if two memory regions \a and \b with a size of \n bytes are equal. When
 * the memory of \a and \b is equal or if both \a and \b are `nullptr`, `true`
 * is returned. Otherwise, `false` is returned. This is like calling `memcmp`,
 * except that it allows for `nullptr` (which would cause undefined behaviour
 * with `memcmp`).
 */
bool
e_mem_eq_size (const void *a, const void *b, size_t n)
{
	if (a == b) return true;
	if (!a || !b) return false;
	return memcmp (a, b, n) == 0;
}

/**
 * Swaps \n bytes of memory from \a and \b. If \a or \b is `nullptr`, no action
 * is performed.
 */
void
e_mem_swap_size (void *a, void *b, size_t n)
{
	uint8_t *ap, *bp, tmp;
	size_t i;

	if (!a || !b) return;
	ap = a;
	bp = b;
	for (i = 0; i < n; i++) {
		tmp = ap[i];
		ap[i] = bp[i];
		bp[i] = tmp;
	}
}

/**
 * Checks if the pointer \ptr is aligned to \align bytes.
 */
bool
e_mem_is_aligned_to (const void *ptr, size_t align)
{
	return ((uintptr_t) ptr % align) == 0;
}

/**
 * Duplicate a string. This is equivalent to `strdup`, except that it terminates
 * the programme in case the memory allocation fails. This function only returns
 * `nullptr` if \s is `nullptr`. The allocated memory must be freed by the user
 * using `e_free`.
 */
char *
e_mem_strdup (const char *s)
{
	size_t len;
	char *res;

	if (!s) return NULL;
	len = strlen (s);
	res = malloc (sizeof (char) * (len + 1));
	if (res == NULL) {
		fprintf (stderr, "[e_mem] failed to strdup %zu bytes\n", len);
		exit (EXIT_FAILURE);
	}
	strcpy (res, s);

	return res;
}

/**
 * Duplicate at most \n bytes of a string. This is equivalent to `strndup`,
 * except that it terminates the programme in case the memory allocation fails.
 * This function only returns `nullptr` if \s is `nullptr`. The allocated memory
 * must be freed by the user using `e_free`. This function only allocates as
 * many bytes as are required, i.e. `min(strlen(s), n) + 1`.
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
	if (res == NULL) {
		fprintf (stderr, "[e_mem] failed to strndup %zu bytes\n", len);
		exit (EXIT_FAILURE);
	}
	strncpy (res, s, len);
	res[len] = 0;

	return res;
}

/**
 * Duplicate \n bytes of memory from the pointer \ptr. The memory is allocated
 * using `e_alloc` and must be freed with `e_free`. If \ptr is `nullptr` or \n
 * is 0, no allocation is performed and `nullptr` is returned.
 */
void *
e_mem_clone_size (const void *ptr, size_t n)
{
	void *ret;

	if (!ptr || n == 0) return NULL;
	ret = malloc (n);
	if (ret == NULL) {
		fprintf (stderr, "[e_mem] failed to alloc %zu bytes\n", n);
		exit (EXIT_FAILURE);
	}
	memcpy (ret, ptr, n);
	return ret;
}

/**
 * Reverse the bytes at the memory location \ptr. \len bytes will be reversed.
 */
void
e_mem_reverse_bytes (void *ptr, size_t len)
{
	uint8_t *start, *end, tmp;

	if (len == 0) return;
	start = ptr;
	end = start + len - 1;
	while (start < end) {
		tmp = *start;
		*start++ = *end;
		*end-- = tmp;
	}
}

/**
 * Read a big-endian `uint16_t` from the memory \mem.
 *
 * Example: { 0x12, 0x34 } => 0x1234
 */
uint16_t
e_mem_read_u16_be (const uint8_t *mem)
{
	return (uint16_t) ((((uint16_t) mem[0]) << 8) | (((uint16_t) mem[1]) << 0));
}

/**
 * Read a little-endian `uint16_t` from the memory \mem.
 *
 * Example: { 0x12, 0x34 } => 0x3412
 */
uint16_t
e_mem_read_u16_le (const uint8_t *mem)
{
	return (uint16_t) ((((uint16_t) mem[1]) << 8) | (((uint16_t) mem[0]) << 0));
}

/**
 * Read a big-endian `uint32_t` from the memory \mem.
 *
 * Example: { 0x12, 0x34, 0x56, 0x78 } => 0x12345678
 */
uint32_t
e_mem_read_u32_be (const uint8_t *mem)
{
	return (((uint32_t) mem[0]) << 24) | (((uint32_t) mem[1]) << 16) |
	       (((uint32_t) mem[2]) <<  8) | (((uint32_t) mem[3]) <<  0);
}

/**
 * Read a little-endian `uint32_t` from the memory \mem.
 *
 * Example: { 0x12, 0x34, 0x56, 0x78 } => 0x78563412
 */
uint32_t
e_mem_read_u32_le (const uint8_t *mem)
{
	return (((uint32_t) mem[3]) << 24) | (((uint32_t) mem[2]) << 16) |
	       (((uint32_t) mem[1]) <<  8) | (((uint32_t) mem[0]) <<  0);
}

/**
 * Read a big-endian `uint64_t` from the memory \mem.
 *
 * Example: { 0x12, 0x34, 0x56, ..., 0x34, 0x56 } => 0x1234567890123456
 */
uint64_t
e_mem_read_u64_be (const uint8_t *mem)
{
	return (((uint64_t) mem[0]) << 56) | (((uint64_t) mem[1]) << 48) |
	       (((uint64_t) mem[2]) << 40) | (((uint64_t) mem[3]) << 32) |
	       (((uint64_t) mem[4]) << 24) | (((uint64_t) mem[5]) << 16) |
	       (((uint64_t) mem[6]) <<  8) | (((uint64_t) mem[7]) <<  0);
}

/**
 * Read a little-endian `uint64_t` from the memory \mem.
 *
 * Example: { 0x12, 0x34, 0x56, ..., 0x34, 0x56 } => 0x5634129078563412
 */
uint64_t
e_mem_read_u64_le (const uint8_t *mem)
{
	return (((uint64_t) mem[7]) << 56) | (((uint64_t) mem[6]) << 48) |
	       (((uint64_t) mem[5]) << 40) | (((uint64_t) mem[4]) << 32) |
	       (((uint64_t) mem[3]) << 24) | (((uint64_t) mem[2]) << 16) |
	       (((uint64_t) mem[1]) <<  8) | (((uint64_t) mem[0]) <<  0);
}

/**
 * Write a big-endian `uint16_t` \value to the memory \mem.
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
 * Write a little-endian `uint16_t` \value to the memory \mem.
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
 * Write a big-endian `uint32_t` \value to the memory \mem.
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
 * Write a little-endian `uint32_t` \value to the memory \mem.
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
 * Write a big-endian `uint64_t` \value to the memory \mem.
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
 * Write a little-endian `uint64_t` \value to the memory \mem.
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

#endif /* E_MEM_IMPL */

#endif /* _EMPOWER_MEM_H_ */
