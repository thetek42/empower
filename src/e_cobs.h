#ifndef _EMPOWER_COBS_H_
#define _EMPOWER_COBS_H_

/*! e_cobs ********************************************************************
 *
 * This module provides an implementation of COBS (Consistent Overhead Byte
 * Stuffing).
 *
 * Configuration options:
 *  - `E_CONFIG_MALLOC_FUNC` (default `malloc`): The function to use for allocating memory.
 *  - `E_CONFIG_FREE_FUNC` (default `free`): The function to use for freeing memory.
 *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

/* compatibility annoyances ***************************************************/

#if !defined (__E_SSIZE_T_DEFINED) && (defined (__MINGW32__) || defined (_WIN32) || defined (WIN32))
# define __E_SSIZE_T_DEFINED
# include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif /* !defined (__E_SSIZE_T_DEFINED) && (defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)) */

/* public interface ***********************************************************/

#define E_COBS_ERR_INVALID_ARG -1
#define E_COBS_ERR_ZERO_IN_ENCODED_DATA -2
#define E_COBS_ERR_TRUNCATED_ENCODED_DATA -3

ssize_t e_cobs_dec (const char *encoded_input, size_t encoded_len, char *output);
char *e_cobs_dec_alloc (const char *encoded_input, size_t encoded_len, size_t *decoded_len_out);
size_t e_cobs_dec_max_output_size (size_t encoded_len);

ssize_t e_cobs_enc (const char *plain_input, size_t plain_len, char *output);
char *e_cobs_enc_alloc (const char *plain_input, size_t plain_len, size_t *encoded_len_out);
size_t e_cobs_enc_max_output_size (size_t plain_len);

/* implementation *************************************************************/

#ifdef E_COBS_IMPL

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef E_CONFIG_MALLOC_FUNC
# define E_CONFIG_MALLOC_FUNC malloc
#endif /* E_CONFIG_MALLOC_FUNC */
#ifndef E_CONFIG_FREE_FUNC
# define E_CONFIG_FREE_FUNC free
#endif /* E_CONFIG_FREE_FUNC */

/**
 * Decode the COBS-encoded string \encoded_input with length \encoded_len. The
 * decoded string will be placed into \output. The user must ensure that \output
 * points to enough memory (i.e. `e_cobs_dec_max_output_size (encoded_len)`
 * bytes). Returns the length of the decoded string on success or a negative
 * number according to the E_COBS_ERR_* macros on error.
 */
ssize_t
e_cobs_dec (const char *encoded_input, size_t encoded_len, char *output)
{
	size_t code_index, in_index, out_index;
	uint8_t code, in_byte;

	if (!encoded_input || !output) return E_COBS_ERR_INVALID_ARG;

	out_index = 0;
	for (code_index = 0; code_index < encoded_len; ) {
		code = (uint8_t) encoded_input[code_index];
		if (code == 0) {
			return E_COBS_ERR_ZERO_IN_ENCODED_DATA;
		}
		in_index = code_index + 1;
		for (; in_index < code_index + (size_t) code; in_index += 1) {
			if (in_index >= encoded_len) {
				return E_COBS_ERR_TRUNCATED_ENCODED_DATA;
			}
			in_byte = encoded_input[in_index];
			if (in_byte == 0) {
				return E_COBS_ERR_ZERO_IN_ENCODED_DATA;
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
 * Decode the COBS-encoded string \encoded_input with length \encoded_len.
 * Allocates enough memory to store the decoded string and returns a pointer
 * that must be freed by the user. If the allocation fails, an error message is
 * printed and the programme is terminated. In case of an error, the previously
 * allocated memory is freed and `nullptr` is returned.
 */
char *
e_cobs_dec_alloc (const char *encoded_input, size_t encoded_len,
                  size_t *decoded_len_out)
{
	size_t max_decoded_len;
	ssize_t decoded_len;
	char *ptr;

	if (!encoded_input) return NULL;

	max_decoded_len = e_cobs_dec_max_output_size (encoded_len);
	ptr = E_CONFIG_MALLOC_FUNC (max_decoded_len);
	if (!ptr) {
		fprintf (stderr, "[e_cobs] failed to alloc %zu bytes\n",
		         max_decoded_len);
		exit (EXIT_FAILURE);
	}

	decoded_len = e_cobs_dec (encoded_input, encoded_len, ptr);

	if (decoded_len < 0) {
		E_CONFIG_FREE_FUNC (ptr);
		return NULL;
	} else {
		if (decoded_len_out) *decoded_len_out = (size_t) decoded_len;
		return ptr;
	}
}

/**
 * Determine the maximum number of bytes required for decoding a COBS-encoded
 * string.
 */
size_t
e_cobs_dec_max_output_size (size_t encoded_len)
{
	if (encoded_len > 1) {
		return encoded_len - 1;
	} else {
		return 0;
	}
}

/**
 * Encode the string \plain_input with length \plain_len using COBS. The encoded
 * string will be placed into \output. The user must ensure that \output points
 * to enough memory (i.e. `e_cobs_enc_max_output_size (encoded_len)` bytes).
 * Returns the length of the decoded string on success or a negative number
 * according to the E_COBS_ERR_* macros on error.
 */
ssize_t
e_cobs_enc (const char *plain_input, size_t plain_len, char *output)
{
	size_t in_index, out_index, code_index;
	char in_char, out_char;

	if (!plain_input || !output) return E_COBS_ERR_INVALID_ARG;

	code_index = 0;
	out_index = 1;
	for (in_index = 0; in_index < plain_len; in_index += 1) {
		in_char = plain_input[in_index];
		if (out_index - code_index >= 0xFF) {
			output[code_index] = (char) (uint8_t) 0xFF;
			code_index = out_index;
			out_index += 1;
		}
		if (in_char == 0) {
			out_char = (char) (uint8_t) (out_index - code_index);
			output[code_index] = out_char;
			code_index = out_index;
			out_index += 1;
		} else {
			output[out_index] = in_char;
			out_index += 1;
		}
	}
	output[code_index] = (char) (uint8_t) (out_index - code_index);

	return (ssize_t) out_index;
}

/**
 * Encode the string \plain_input with length \plain_len using COBS. Allocates
 * enough memory to store the decoded string and returns a pointer that must be
 * freed by the user. If the allocation fails, an error message is printed and
 * the programme is terminated. In case of an error, the previously allocated
 * memory is freed and `nullptr` is returned.
 */
char *
e_cobs_enc_alloc (const char *plain_input, size_t plain_len,
                  size_t *encoded_len_out)
{
	size_t max_encoded_len;
	ssize_t encoded_len;
	char *ptr;

	if (!plain_input) return NULL;

	max_encoded_len = e_cobs_enc_max_output_size (plain_len);
	ptr = E_CONFIG_MALLOC_FUNC (max_encoded_len);
	if (!ptr) {
		fprintf (stderr, "[e_cobs] failed to alloc %zu bytes\n",
		         max_encoded_len);
		exit (EXIT_FAILURE);
	}

	encoded_len = e_cobs_enc (plain_input, plain_len, ptr);

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
 * COBS.
 */
size_t
e_cobs_enc_max_output_size (size_t plain_len)
{
	if (plain_len == 0) {
		return 1;
	} else {
		return plain_len + (plain_len + 253) / 254;
	}
}

#endif /* E_COBS_IMPL */

#endif /* _EMPOWER_COBS_H_ */
