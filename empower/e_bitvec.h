#ifndef EMPOWER_BITVEC_H_
#define EMPOWER_BITVEC_H_

/******************************************************************************
 *
 * This module provides a freestanding implementation of a "bit vector", i.e. an
 * array that allows for convenient access to a large number of bits. It can be
 * used as an efficient implementation of arrays of booleans. The bit vectors
 * provided by this implementation are not resizable, since that is not really
 * necessary in most cases. So technically, these should be called "bit arrays"
 * instead, but who cares.
 *
 ******************************************************************************/

#include <stddef.h>

/**
 * A bit vector, consisting of the \data pointer and \cap, the number of
 * available bits.
 */
struct e_bitvec {
	unsigned char *data;
	size_t cap;
};

struct e_bitvec e_bitvec_init (unsigned char *data, size_t cap);
int e_bitvec_get (const struct e_bitvec *bitvec, size_t index);
int e_bitvec_all (const struct e_bitvec *bitvec, size_t start, size_t end);
int e_bitvec_any (const struct e_bitvec *bitvec, size_t start, size_t end);
void e_bitvec_set (struct e_bitvec *bitvec, size_t index);
void e_bitvec_unset (struct e_bitvec *bitvec, size_t index);
void e_bitvec_put (struct e_bitvec *bitvec, size_t index, int value);
void e_bitvec_negate (struct e_bitvec *bitvec, size_t index);

/******************************************************************************/

#ifdef E_BITVEC_IMPL

#include <string.h>

/**
 * Initialise a bit vector with a pointer `data` that allows storing `cap` BITS
 * (not bytes!) of data. `cap` must be divisible by 8. This means that `data`
 * must point to `cap / 8` items of type `unsigned char`. All elements are
 * initialised to 0.
 */
struct e_bitvec
e_bitvec_init (unsigned char *data,
	       size_t cap)
{
	struct e_bitvec ret;
	memset (data, 0, cap / 8);
	ret.data = data;
	ret.cap = cap;
	return ret;
}

/**
 * Get a bit at `index` within the bit vector `bitvec`. Returns 0 or 1,
 * depending on the value of the bit. If `index` is out of range, 0 is returned.
 */
int
e_bitvec_get (const struct e_bitvec *bitvec,
	      size_t index)
{
	unsigned char byte;
	if (index >= bitvec->cap) return 1;
	byte = bitvec->data[index / 8];
	return (byte >> (index % 8)) & 0x1;
}

/**
 * Check if all bits within a range in `bitvec` are set to 1.
 *
 * The range is given by `start` (inclusive) and `end` (exclusive). If all bits
 * within that range are 1, 1 is returned, 0 otherwise.
 *
 * If `start` is out of range, 0 is returned. If `end` is out of range, it is
 * trimmed to the capacity of the bit vector. If the range has a length of 0,
 * the value 1 is returned.
 */
int
e_bitvec_all (const struct e_bitvec *bitvec,
	      size_t start,
	      size_t end)
{
	unsigned char byte;
	size_t i;

	if (start > end) return 0;
	if (start >= bitvec->cap) return 0;
	end = end >= bitvec->cap ? bitvec->cap - 1 : end;
	for (i = start; i < end; i++) {
		byte = bitvec->data[i / 8];
		if (!(byte & (1 << (i % 8)))) {
			return 0;
		}
	}
	return 1;
}

/**
 * Check if any of the bits within a range in `bitvec` are set to 1.
 *
 * The range is given by `start` (inclusive) and `end` (exclusive). If at least
 * one bit within that range is 1, 1 is returned, 0 otherwise.
 *
 * If `start` is out of range, 0 is returned. If `end` is out of range, it is
 * trimmed to the capacity of the bit vector. If the range has a length of 0,
 * the value 0 is returned.
 */
int
e_bitvec_any (const struct e_bitvec *bitvec,
              size_t start,
              size_t end)
{
	unsigned char byte;
	size_t i;

	if (start > end) return 0;
	if (start >= bitvec->cap) return 0;
	end = end >= bitvec->cap ? bitvec->cap - 1 : end;
	for (i = start; i < end; i++) {
		byte = bitvec->data[i / 8];
		if (byte & (1 << (i % 8))) {
			return 1;
		}
	}
	return 0;
}

/**
 * Set the bit at `index` within the bit vector `bitvec` to 1.
 * Does nothing if `index` is out of range.
 */
void
e_bitvec_set (struct e_bitvec *bitvec,
              size_t index)
{
	unsigned char *byte;
	if (index >= bitvec->cap) return;
	byte = &bitvec->data[index / 8];
	*byte |= (unsigned char) (1 << (index % 8));
}

/**
 * Set the bit at `index` within the bit vector `bitvec` to 0.
 * Does nothing if `index` is out of range.
 */
void
e_bitvec_unset (struct e_bitvec *bitvec,
                size_t index)
{
	unsigned char *byte;
	if (index >= bitvec->cap) return;
	byte = &bitvec->data[index / 8];
	*byte &= (unsigned char) ~(1 << (index % 8));
}

/**
 * Set the bit at `index` within the bit vector `bitvec` to `value`.
 * Does nothing if `index` is out of range.
 */
void
e_bitvec_put (struct e_bitvec *bitvec,
              size_t index,
              int value)
{
	if (value) e_bitvec_set (bitvec, index);
	else e_bitvec_unset (bitvec, index);
}

/**
 * Negate the bit at `index` within the bit vector `bitvec`.
 * Does nothing if `index` is out of range.
 */
void
e_bitvec_negate (struct e_bitvec *bitvec,
                 size_t index)
{
	unsigned char *byte;
	if (index >= bitvec->cap) return;
	byte = &bitvec->data[index / 8];
	*byte ^= (unsigned char) (1 << (index % 8));
}

#endif /* E_BITVEC_IMPL */

#endif /* EMPOWER_BITVEC_H_ */
