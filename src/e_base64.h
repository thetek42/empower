#ifndef _EMPOWER_BASE64_H_
#define _EMPOWER_BASE64_H_

/*! e_base64 ******************************************************************
 * 
 * This module provides functions for encoding and decoding base64.
 *
 * Example:
 *  | #define E_BASE64_IMPL
 *  | #include <e_base64.h>
 *  |
 *  | char *plain = "Hello, World!\n";
 *  | char *encoded = e_base64_enc_alloc (plain);
 *  | if (!encoded) return E_FAIL;
 *  | e_log_debug ("Encoded: %s", encoded);
 *  | e_free (encoded);
 *
 * Configuration options:
 *  - `E_CONFIG_MALLOC_FUNC` (default `malloc`): The function to use for allocating memory.
 *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>

/* public interface ***********************************************************/

size_t e_base64_get_enc_len (const char *plain);
size_t e_base64_get_dec_len (const char *encoded);
void e_base64_enc (const char *plain, char *encoded_out);
bool e_base64_dec (const char *encoded, char *plain_out);
char *e_base64_enc_alloc (const char *plain);
char *e_base64_dec_alloc (const char *encoded);

/* implementation *************************************************************/

#ifdef E_BASE64_IMPL

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef E_CONFIG_MALLOC_FUNC
# define E_CONFIG_MALLOC_FUNC malloc
#endif /* E_CONFIG_MALLOC_FUNC */

static bool base64_is_valid_char (char c);

static const char base64_enc_lut[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char base64_dec_lut[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 62, 62,
	63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 0, 0, 0, 0, 63, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
};

/**
 * Get the length required to store the result of Base64-encoding a
 * nul-terminated plain text \plain. The returned length does not include the
 * terminating nul byte.
 */
size_t
e_base64_get_enc_len (const char *plain)
{
	size_t ret, len;

	ret = len = strlen (plain);
	ret += len % 3 ? 3 - len % 3 : 0;
	ret = ret / 3 * 4;

	return ret;
}

/**
 * Get the length required to store the result of Base64-decoding the
 * nul-terminated encoded text \encoded. The length of \encoded is determined
 * using `strlen`. The returned length does not include the terminating nul
 * byte.
 */
size_t
e_base64_get_dec_len (const char *encoded)
{
	size_t len, ret, i;

	if (!encoded) return 0;

	len = strlen (encoded);
	ret = len / 4 * 3;

	for (i = len; i > 0; i--) {
		if (encoded[i - 1] == '=') ret -= 1;
		else break;
	}

	return ret;
}

/**
 * Encode a nul-terminated plain text \plain using Base64 and store the result
 * in \encoded_out. \encoded_out must be capable of holding at least
 * `e_base64_get_enc_len (encoded) + 1` bytes (including the nul terminator).
 */
void
e_base64_enc (const char *plain, char *encoded_out)
{
	size_t i, j, len;
	uint32_t n;

	if (!encoded_out) return;
	if (!plain) {
		encoded_out[0] = 0;
		return;
	}

	len = strlen (plain);
	for (i = 0, j = 0; i < len; i += 3, j += 4) {
		n = (uint32_t) plain[i];
		n = i + 1 < len ? n << 8 | (uint32_t) plain[i + 1] : n << 8;
		n = i + 2 < len ? n << 8 | (uint32_t) plain[i + 2] : n << 8;

		encoded_out[j] = base64_enc_lut[(n >> 18) & 0x3F];
		encoded_out[j + 1] = base64_enc_lut[(n >> 12) & 0x3F];
		if (i + 1 < len) {
			encoded_out[j + 2] = base64_enc_lut[(n >> 6) & 0x3F];
		} else {
			encoded_out[j + 2] = '=';
		}
		if (i + 2 < len) {
			encoded_out[j + 3] = base64_enc_lut[n & 0x3F];
		} else {
			encoded_out[j + 3] = '=';
		}
	}
	encoded_out[j] = 0;
}

/**
 * Decode a nul-terminated Base64-encoded text \encoded and store the result in
 * \plain_out. \plain_out must be capable of holding at least
 * `e_base64_get_dec_len (encoded) + 1` bytes (including the nul terminator).
 * Returns true when the decoding was successful.
 */
bool
e_base64_dec (const char *encoded, char *plain_out)
{
	size_t i, j, len;
	uint32_t n;

	if (!plain_out) return false;
	if (!encoded) {
		plain_out[0] = 0;
		return true;
	}

	len = strlen (encoded);
	if (len % 4 != 0) return false;

	for (i = 0, j = 0; i < len; i += 4) {
		if (!base64_is_valid_char(encoded[i])) return false;

		n = (uint32_t) base64_dec_lut[(uint8_t) encoded[i]];
		n = (n << 6) | (uint32_t) base64_dec_lut[(uint8_t) encoded[i + 1]];
		n = encoded[i + 2] == '=' ? n << 6 : (n << 6) | (uint32_t) base64_dec_lut[(uint8_t) encoded[i + 2]];
		n = encoded[i + 3] == '=' ? n << 6 : (n << 6) | (uint32_t) base64_dec_lut[(uint8_t) encoded[i + 3]];

		plain_out[j++] = (char) (n >> 16);
		if (encoded[i + 2] != '=') plain_out[j++] = (char) (n >> 8);
		if (encoded[i + 3] != '=') plain_out[j++] = (char) n;
	}
	plain_out[j] = 0;

	return true;
}

/**
 * Encode a nul-terminated plain text \plain using Base64, allocates enough
 * memory for storing the result and stores the encoded text including a nul
 * terminator in there. A pointer to the allocated memory is returned. The
 * memory must be freed by the user.
 */
char *
e_base64_enc_alloc (const char *plain)
{
	size_t buf_len;
	char *buf;

	buf_len = e_base64_get_enc_len (plain) + 1;
	buf = E_CONFIG_MALLOC_FUNC (sizeof (char) * buf_len);
	if (!buf) {
		fprintf (stderr, "[e_enc] failed to alloc %zu bytes\n", buf_len);
		exit (EXIT_FAILURE);
	}
	e_base64_enc (plain, buf);

	return buf;
}

/**
 * Decode a nul-terminated Base64-encoded text \encoded, allocates enough memory
 * for storing the result and stores the decoded text including a nul terminator
 * in there. A pointer to the allocated memory is returned. If the decoding
 * fails, `nullptr` is returned. A pointer to the allocated memory is returned.
 * The memory must be freed by the user.
 */
char *
e_base64_dec_alloc (const char *encoded)
{
	size_t buf_len;
	bool success;
	char *buf;

	buf_len = e_base64_get_enc_len (encoded) + 1;
	buf = E_CONFIG_MALLOC_FUNC (sizeof (char) * buf_len);
	if (!buf) {
		fprintf (stderr, "[e_enc] failed to alloc %zu bytes\n", buf_len);
		exit (EXIT_FAILURE);
	}
	success = e_base64_dec (encoded, buf);
	if (!success) {
		free (buf);
		buf = NULL;
	}

	return buf;
}

static bool
base64_is_valid_char (char c)
{
	return isalnum (c) || c == '/' || c == '+' || c == '=';
}

#endif /* E_BASE64_IMPL */

#endif /* _EMPOWER_BASE64_H_ */
