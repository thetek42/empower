#ifndef _EMPOWER_COBSR_H_
#define _EMPOWER_COBSR_H_

/*! e_cobsr ********************************************************************
 *
 * This module provides an implementation of COBS/R (Consistent Overhead Byte
 * Stuffing Reduced).
 *
 * Configuration options:
 *  - `E_CONFIG_MALLOC_FUNC` (default `malloc`): The function to use for allocating memory.
 *  - `E_CONFIG_FREE_FUNC` (default `free`): The function to use for freeing memory.
 *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

/* compatibility annoyances ***************************************************/

#if !defined (__E_SSIZE_T_DEFINED) && (defined (__MINGW32__) || defined (_WIN32) || defined (WIN32))
# define __E_SSIZE_T_DEFINED
# include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif /* !defined (__E_SSIZE_T_DEFINED) && (defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)) */

/* public interface ***********************************************************/

#define E_COBSR_ERR_INVALID_ARG -1
#define E_COBSR_ERR_ZERO_IN_ENCODED_DATA -2
#define E_COBSR_ERR_TRUNCATED_ENCODED_DATA -3

ssize_t e_cobsr_dec (const uint8_t *encoded_input, size_t encoded_len, uint8_t *output);
uint8_t *e_cobsr_dec_alloc (const uint8_t *encoded_input, size_t encoded_len, size_t *decoded_len_out);
size_t e_cobsr_dec_max_output_size (size_t encoded_len);

ssize_t e_cobsr_enc (const uint8_t *plain_input, size_t plain_len, uint8_t *output);
uint8_t *e_cobsr_enc_alloc (const uint8_t *plain_input, size_t plain_len, size_t *encoded_len_out);
size_t e_cobsr_enc_max_output_size (size_t plain_len);

/* implementation *************************************************************/

#ifdef E_COBSR_IMPL

#include <stdio.h>
#include <stdlib.h>

#ifndef E_CONFIG_MALLOC_FUNC
# define E_CONFIG_MALLOC_FUNC malloc
#endif /* E_CONFIG_MALLOC_FUNC */
#ifndef E_CONFIG_FREE_FUNC
# define E_CONFIG_FREE_FUNC free
#endif /* E_CONFIG_FREE_FUNC */

/**
 * Decode the COBS/R-encoded string \encoded_input with length \encoded_len. The
 * decoded string will be placed into \output. The user must ensure that \output
 * points to enough memory (i.e. `e_cobsr_dec_max_output_size (encoded_len)`
 * bytes). Returns the length of the decoded string on success or a negative
 * number according to the E_COBSR_ERR_* macros on error.
 */
ssize_t
e_cobsr_dec (const uint8_t *encoded_input, size_t encoded_len, uint8_t *output)
{
	size_t code_index, in_index, out_index;
	uint8_t code, in_byte;

	if (!encoded_input || !output) return E_COBSR_ERR_INVALID_ARG;

	out_index = 0;
	for (code_index = 0; code_index < encoded_len; ) {
		code = encoded_input[code_index];
		if (code == 0) {
			return E_COBSR_ERR_ZERO_IN_ENCODED_DATA;
		}
		in_index = code_index + 1;
		for (; in_index < code_index + (size_t) code; in_index += 1) {
			if (in_index >= encoded_len) {
				output[out_index] = code;
				out_index += 1;
				break;
			}
			in_byte = encoded_input[in_index];
			if (in_byte == 0) {
				return E_COBSR_ERR_ZERO_IN_ENCODED_DATA;
			}
			output[out_index] = in_byte;
			out_index += 1;
		}
		code_index += (size_t) code;
		if (code_index >= encoded_len) break;
		if (code < 0xFF) {
			output[out_index] = 0;
			out_index += 1;
		}
	}

	return (ssize_t) out_index;
}

/**
 * Decode the COBS/R-encoded string \encoded_input with length \encoded_len.
 * Allocates enough memory to store the decoded string and returns a pointer
 * that must be freed by the user. If the allocation fails, an error message is
 * printed and the programme is terminated. In case of an error, the previously
 * allocated memory is freed and `nullptr` is returned. The length of the
 * decoded string is stored in \decoded_len_out.
 */
uint8_t *
e_cobsr_dec_alloc (const uint8_t *encoded_input, size_t encoded_len,
                  size_t *decoded_len_out)
{
	size_t max_decoded_len;
	ssize_t decoded_len;
	uint8_t *ptr;

	if (!encoded_input) return NULL;

	max_decoded_len = e_cobsr_dec_max_output_size (encoded_len);
	ptr = E_CONFIG_MALLOC_FUNC (max_decoded_len);
	if (!ptr) {
		fprintf (stderr, "[e_cobsr] failed to alloc %zu bytes\n",
		         max_decoded_len);
		exit (EXIT_FAILURE);
	}

	decoded_len = e_cobsr_dec (encoded_input, encoded_len, ptr);

	if (decoded_len < 0) {
		E_CONFIG_FREE_FUNC (ptr);
		return NULL;
	} else {
		if (decoded_len_out) *decoded_len_out = (size_t) decoded_len;
		return ptr;
	}
}

/**
 * Determine the maximum number of bytes required for decoding a COBS/R-encoded
 * string.
 */
size_t
e_cobsr_dec_max_output_size (size_t encoded_len)
{
	return encoded_len;
}

/**
 * Encode the string \plain_input with length \plain_len using COBS/R. The
 * encoded string will be placed into \output. The user must ensure that \output
 * points to enough memory (i.e. `e_cobsr_enc_max_output_size (encoded_len)`
 * bytes). Returns the length of the decoded string on success or a negative
 * number according to the E_COBSR_ERR_* macros on error.
 */
ssize_t
e_cobsr_enc (const uint8_t *plain_input, size_t plain_len, uint8_t *output)
{
	size_t in_index, out_index, code_index;
	uint8_t in_char, out_char, last_value;

	if (!plain_input || !output) return E_COBSR_ERR_INVALID_ARG;

	code_index = 0;
	out_index = 1;
	last_value = 0;
	for (in_index = 0; in_index < plain_len; in_index += 1) {
		in_char = plain_input[in_index];
		if (out_index - code_index >= 0xFF) {
			output[code_index] = 0xFF;
			code_index = out_index;
			out_index += 1;
		}
		if (in_char == 0) {
			out_char = (uint8_t) (out_index - code_index);
			output[code_index] = out_char;
			code_index = out_index;
			out_index += 1;
			last_value = 0;
		} else {
			output[out_index] = in_char;
			last_value = in_char;
			out_index += 1;
		}
	}

	out_char = (uint8_t) (out_index - code_index);
	if (last_value >= out_char) {
		output[code_index] = last_value;
		out_index -= 1;
	} else {
		output[code_index] = out_char;
	}

	return (ssize_t) out_index;
}

/**
 * Encode the string \plain_input with length \plain_len using COBS/R. Allocates
 * enough memory to store the decoded string and returns a pointer that must be
 * freed by the user. If the allocation fails, an error message is printed and
 * the programme is terminated. In case of an error, the previously allocated
 * memory is freed and `nullptr` is returned. The length of the encoded string
 * is stored in \decoded_len_out.
 */
uint8_t *
e_cobsr_enc_alloc (const uint8_t *plain_input, size_t plain_len,
                   size_t *encoded_len_out)
{
	size_t max_encoded_len;
	ssize_t encoded_len;
	uint8_t *ptr;

	if (!plain_input) return NULL;

	max_encoded_len = e_cobsr_enc_max_output_size (plain_len);
	ptr = E_CONFIG_MALLOC_FUNC (max_encoded_len);
	if (!ptr) {
		fprintf (stderr, "[e_cobsr] failed to alloc %zu bytes\n",
		         max_encoded_len);
		exit (EXIT_FAILURE);
	}

	encoded_len = e_cobsr_enc (plain_input, plain_len, ptr);

	if (encoded_len < 0) {
		E_CONFIG_FREE_FUNC (ptr);
		return NULL;
	} else {
		if (encoded_len_out) *encoded_len_out = (size_t) encoded_len;
		return ptr;
	}
}

/**
 * Determine the maximum number of bytes required for encoding a string with
 * COBS/R.
 */
size_t
e_cobsr_enc_max_output_size (size_t plain_len)
{
	if (plain_len == 0) {
		return 1;
	} else {
		return plain_len + (plain_len + 253) / 254;
	}
}

#endif /* E_COBSR_IMPL */

#endif /* _EMPOWER_COBSR_H_ */
