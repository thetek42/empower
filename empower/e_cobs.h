#ifndef EMPOWER_COBS_H_
#define EMPOWER_COBS_H_

/******************************************************************************
 *
 * This module provides a free-standing implementation of COBS (Consistent
 * Overhead Byte Stuffing). COBS is a simple, efficient algorithm for packet
 * framing that does not result in arbitrary byte stuffing overhead. For more
 * information about COBS, check out its Wikipedia article:
 *   https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
 *
 ******************************************************************************/

#include <stddef.h>

enum e_cobs_result {
	E_COBS_OK = 0,
	E_COBS_ERR_INVALID_ARG = -1,
	E_COBS_ERR_ZERO_IN_ENCODED_DATA = -2,
	E_COBS_ERR_TRUNCATED_ENCODED_DATA = -3
};

enum e_cobs_result e_cobs_decode (const unsigned char *encoded_input,
                                  size_t encoded_len,
                                  unsigned char *output,
                                  size_t *output_len);

size_t e_cobs_decode_output_size (size_t encoded_len);

enum e_cobs_result e_cobs_encode (const unsigned char *plain_input,
                                  size_t plain_len,
                                  unsigned char *output,
                                  size_t *output_len);

size_t e_cobs_encode_output_size (size_t plain_len);

/******************************************************************************/

#ifdef E_COBS_IMPL

/**
 * Decode a COBS-encoded string.
 *
 * The decoded string of length `encoded_len` has to be given `encoded_input`.
 * The output will be placed in `output`, and the length of the decoded output
 * will be written to `output_len`. The user must ensure that `output` is big
 * enough. For finding out how much space is required for the output, the
 * function `e_cobs_decode_output_size` can be used.
 *
 * On success, E_COBS_OK is returned. For invalid data or invalid parameters,
 * an error is returned.
 */
enum e_cobs_result
e_cobs_decode (const unsigned char *encoded_input,
               size_t encoded_len,
               unsigned char *output,
               size_t *output_len)
{
	size_t code_index, in_index, out_index;
	unsigned char code, in_byte;

	if (encoded_input == NULL || output == NULL || output_len == NULL) {
		return E_COBS_ERR_INVALID_ARG;
	}

	out_index = 0;
	for (code_index = 0; code_index < encoded_len; ) {
		code = encoded_input[code_index];
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

	*output_len = out_index;
	return E_COBS_OK;
}

/**
 * Determine the maximum number of bytes required for decoding a COBS-encoded
 * string.
 */
size_t
e_cobs_decode_output_size (size_t encoded_len)
{
	if (encoded_len == 0) return 0;
	return encoded_len - 1;
}

/**
 * Encode a string using COBS.
 *
 * The string of length `plain_len` has to be given in `plain_input`. The output
 * will be placed in `output`, and the length of the encoded output will be
 * written to `output_len`. A trailing 0 byte is NOT written. The user must
 * ensure that `output` is big enough. For finding out how much space is
 * required for the output, the function `e_cobs_encode_output_size` can be
 * used.
 *
 * On success, E_COBS_OK is returned. For invalid data or invalid parameters,
 * an error is returned.
 */
enum e_cobs_result
e_cobs_encode (const unsigned char *plain_input,
               size_t plain_len,
               unsigned char *output,
               size_t *output_len)
{
	size_t in_index, out_index, code_index;
	unsigned char in_char, out_char;

	if (plain_input == NULL || output == NULL || output_len == NULL) {
		return E_COBS_ERR_INVALID_ARG;
	}

	code_index = 0;
	out_index = 1;
	for (in_index = 0; in_index < plain_len; in_index += 1) {
		in_char = plain_input[in_index];
		if (out_index - code_index >= 0xFF) {
			output[code_index] = 0xFF;
			code_index = out_index;
			out_index += 1;
		}
		if (in_char == 0) {
			out_char = (unsigned char) (out_index - code_index);
			output[code_index] = out_char;
			code_index = out_index;
			out_index += 1;
		} else {
			output[out_index] = in_char;
			out_index += 1;
		}
	}
	output[code_index] = (unsigned char) (out_index - code_index);

	*output_len = out_index;
	return E_COBS_OK;
}

/**
 * Determine the maximum number of bytes required for encoding a string with
 * COBS.
 */
size_t
e_cobs_encode_output_size (size_t plain_len)
{
	if (plain_len == 0) return 1;
	return plain_len + ((plain_len + 253) / 254);
}

#endif /* E_COBS_IMPL */

#endif /* EMPOWER_COBS_H_ */
