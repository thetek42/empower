#ifndef _EMPOWER_BITVEC_H_
#define _EMPOWER_BITVEC_H_

/*! e_bitvec ******************************************************************
 *
 * This module provides an implementation of a "bit vector", i.e. an array that
 * allows for convenient access to a large number of bits. It can be used as an
 * efficient implementation of arrays of booleans. The bit vectors provided by
 * this implementation are not resizable, since static memory is not really
 * necessary in most cases. So technically, these should be called "bit arrays"
 * instead, but who cares.
 *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* public interface ***********************************************************/

/**
 * A bit vector, consisting of the \data pointer and \cap, the number of
 * available bits.
 */
typedef struct {
	uint8_t *data;
	size_t cap;
} E_Bitvec;

E_Bitvec e_bitvec_init (uint8_t *data, size_t cap);
bool e_bitvec_get (const E_Bitvec *bitvec, size_t index);
bool e_bitvec_all (const E_Bitvec *bitvec, size_t start, size_t end);
bool e_bitvec_any (const E_Bitvec *bitvec, size_t start, size_t end);
void e_bitvec_set (E_Bitvec *bitvec, size_t index);
void e_bitvec_unset (E_Bitvec *bitvec, size_t index);
void e_bitvec_put (E_Bitvec *bitvec, size_t index, bool value);
void e_bitvec_negate (E_Bitvec *bitvec, size_t index);

/* implementation *************************************************************/

#ifdef E_BITVEC_IMPL

#include <assert.h>
#include <string.h>

/**
 * Initialise a bit vector with a pointer \data that allows storing \cap BITS of
 * data.
 */
E_Bitvec
e_bitvec_init (uint8_t *data, size_t cap)
{
	size_t excess;

	assert (data && "data for bitvec cannot be null");
	excess = (8 - (cap % 8)) % 8;
	cap += excess;
	memset (data, 0, cap / 8);

	return (E_Bitvec) {
		.data = data,
		.cap = cap,
	};
}

/**
 * Get a bit at \index within the bit vector \bitvec. If \bitvec is `nullptr` or
 * \index is out of range, `false` is returned.
 */
bool
e_bitvec_get (const E_Bitvec *bitvec, size_t index)
{
	if (!bitvec) return false;
	if (index >= bitvec->cap) return false;
	return (((bitvec->data[index / 8]) >> (index % 8)) & 0x1) == 1;
}

/**
 * Check if all bits within the range \start (inclusive) to \end (exclusive)
 * within the bit vector \bitvec are 1. If \bitvec is `nullptr` or \start is out
 * of range, `false` is returned. If \end is out of range, it is trimmed to the
 * capacity of the bit vector. If the range is 0 bits long, `true` is returned.
 */
bool
e_bitvec_all (const E_Bitvec *bitvec, size_t start, size_t end)
{
	size_t i;
	if (!bitvec) return false;
	if (start > end) return false;
	if (start >= bitvec->cap) return false;
	end = end >= bitvec->cap ? bitvec->cap - 1 : end;
	for (i = start; i < end; i++) {
		if (!(bitvec->data[i / 8] & (1 << (i % 8)))) {
			return false;
		}
	}
	return true;
}

/**
 * Check if any of the bits within the range \start (inclusive) to \end
 * (exclusive) within the bit vector \bitvec are 1. If \bitvec is `nullptr` or
 * \start is out of range, `false` is returned. If \end is out of range, it is
 * trimmed to the capacity of the bit vector. If the range is 0 bits long,
 * `false` is returned.
 */
bool
e_bitvec_any (const E_Bitvec *bitvec, size_t start, size_t end)
{
	size_t i;
	if (!bitvec) return false;
	if (start > end) return false;
	if (start >= bitvec->cap) return false;
	end = end >= bitvec->cap ? bitvec->cap - 1 : end;
	for (i = start; i < end; i++) {
		if (bitvec->data[i / 8] & (1 << (i % 8))) {
			return true;
		}
	}
	return false;
}

/**
 * Set the bit at \index within the bit vector \bitvec to 1.
 */
void
e_bitvec_set (E_Bitvec *bitvec, size_t index)
{
	if (!bitvec) return;
	if (index >= bitvec->cap) return;
	bitvec->data[index / 8] |= (uint8_t) (1 << (index % 8));
}

/**
 * Set the bit at \index within the bit vector \bitvec to 0.
 */
void
e_bitvec_unset (E_Bitvec *bitvec, size_t index)
{
	if (!bitvec) return;
	if (index >= bitvec->cap) return;
	bitvec->data[index / 8] &= (uint8_t) ~(1 << (index % 8));
}

/**
 * Set the bit at \index within the bit vector \bitvec to \value.
 */
void
e_bitvec_put (E_Bitvec *bitvec, size_t index, bool value)
{
	if (value) e_bitvec_set (bitvec, index);
	else e_bitvec_unset (bitvec, index);
}

/**
 * Negate the bit at \index within the bit vector \bitvec.
 */
void
e_bitvec_negate (E_Bitvec *bitvec, size_t index)
{
	if (!bitvec) return;
	if (index >= bitvec->cap) return;
	bitvec->data[index / 8] ^= (uint8_t) (1 << (index % 8));
}

#endif /* E_BITVEC_IMPL */

#endif /* _EMPOWER_BITVEC_H_ */
