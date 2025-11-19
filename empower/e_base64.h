#ifndef EMPOWER_BASE64_H_
#define EMPOWER_BASE64_H_

/******************************************************************************
 *
 * This module provides a freestanding implementation for encoding and decoding
 * base64 text. It uses the characters [a-zA-Z0-9+/], which is the most
 * wide-spread base64 character set.
 *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>

size_t e_base64_encoded_len (size_t plain_len);

size_t e_base64_decoded_len (const unsigned char *encoded,
                             size_t encoded_len);

size_t e_base64_encode (const unsigned char *plain,
                        size_t plain_len,
                        unsigned char *encoded_out);

int e_base64_decode (const unsigned char *encoded,
                     size_t encoded_len,
                     unsigned char *plain_out,
                     size_t *plain_len);

/* implementation *************************************************************/

#ifdef E_BASE64_IMPL

static int base64_is_valid_char_ (unsigned char c);

static const unsigned char base64_enc_lut_[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

static const unsigned char base64_dec_lut_[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 62, 62,
	63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 0, 0, 0, 0, 63, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

/**
 * Get the length required to store the result of Base64-encoding a plain text
 * of length `plain_len`. The returned length does not include a terminating
 * nul byte.
 */
size_t
e_base64_encoded_len (size_t plain_len)
{
	size_t ret;

	ret = plain_len;
	ret += plain_len % 3 ? 3 - (plain_len % 3) : 0;
	ret = ret / 3 * 4;

	return ret;
}

/**
 * Get the length required to store the result of Base64-decoding the
 * nul-terminated encoded text `encoded` of length `encoded_len`. The returned
 * length does not include a terminating nul byte.
 */
size_t
e_base64_decoded_len (const unsigned char *encoded,
                      size_t encoded_len)
{
	size_t ret, i;

	if (!encoded) return 0;

	ret = (encoded_len / 4) * 3;

	for (i = encoded_len; i > 0; i--) {
		if (encoded[i - 1] == '=') ret -= 1;
		else break;
	}

	return ret;
}

/**
 * Encode a plain text `plain` of length `plain_len` using Base64 and store the
 * result in `encoded_out`. `encoded_out` must be capable of holding at least
 * `e_base64_get_enc_len (plain_len)` bytes. A nul terminator is not written.
 * Returns the number of bytes written.
 */
size_t
e_base64_encode (const unsigned char *plain,
                 size_t plain_len,
                 unsigned char *encoded_out)
{
	size_t i, j;
	unsigned long n;

	if (encoded_out == NULL || plain == NULL || plain_len == 0) {
		return 0;
	}

	for (i = 0, j = 0; i < plain_len; i += 3, j += 4) {
		n = (unsigned long) plain[i];
		n = i + 1 < plain_len ? n << 8 | (unsigned long) plain[i + 1] : n << 8;
		n = i + 2 < plain_len ? n << 8 | (unsigned long) plain[i + 2] : n << 8;

		encoded_out[j] = base64_enc_lut_[(n >> 18) & 0x3F];
		encoded_out[j + 1] = base64_enc_lut_[(n >> 12) & 0x3F];
		if (i + 1 < plain_len) {
			encoded_out[j + 2] = base64_enc_lut_[(n >> 6) & 0x3F];
		} else {
			encoded_out[j + 2] = '=';
		}
		if (i + 2 < plain_len) {
			encoded_out[j + 3] = base64_enc_lut_[n & 0x3F];
		} else {
			encoded_out[j + 3] = '=';
		}
	}

        return j;
}

/**
 * Decode a nul-terminated Base64-encoded text `encoded` of length `encoded_len`
 * and store the result in `plain_out`. `plain_out` must be capable of holding
 * at least `e_base64_get_dec_len (encoded)` bytes. A nul terminator is not
 * written. The number of written byters is placed in `plain_len`. Returns
 * non-zero when the decoding was successful.
 */
int
e_base64_decode (const unsigned char *encoded,
                 size_t encoded_len,
                 unsigned char *plain_out,
                 size_t *plain_len)
{
	unsigned long n;
	size_t i, j;

        if (plain_out == NULL || plain_len == NULL) return 0;
	if (encoded == NULL || encoded_len == 0) return 1;
	if (encoded_len % 4 != 0) return 0;

	for (i = 0, j = 0; i < encoded_len; i += 4) {
		if (!base64_is_valid_char_(encoded[i])) return 0;

		n = (unsigned long) base64_dec_lut_[(unsigned char) encoded[i]];
                n <<= 6;
		n |= (unsigned long) base64_dec_lut_[(unsigned char) encoded[i + 1]];
                n <<= 6;
		if (encoded[i + 2] != '=') {
                        n |= (unsigned long) base64_dec_lut_[(unsigned char) encoded[i + 2]];
                }
                n <<= 6;
		if (encoded[i + 3] != '=') {
                        n |= (unsigned long) base64_dec_lut_[(unsigned char) encoded[i + 3]];
                }

		plain_out[j++] = (unsigned char) (n >> 16);
		if (encoded[i + 2] != '=') plain_out[j++] = (unsigned char) (n >> 8);
		if (encoded[i + 3] != '=') plain_out[j++] = (unsigned char) n;
	}

	*plain_len = j;
	return 1;
}

static int
base64_is_valid_char_ (unsigned char c)
{
        if ('a' <= c && c <= 'z') return 1;
        if ('A' <= c && c <= 'Z') return 1;
        if ('0' <= c && c <= '9') return 1;
	return c == '/' || c == '+' || c == '=';
}

#endif /* E_BASE64_IMPL */

#endif /* EMPOWER_BASE64_H_ */
