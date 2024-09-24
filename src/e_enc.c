#include "empower.h"

#if E_CONFIG_MODULE_ENC

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

usize
e_base64_get_enc_len (const char *plain)
{
	usize ret, len;

	ret = len = strlen (plain);
	ret += len % 3 ? 3 - len % 3 : 0;
	ret = ret / 3 * 4;

	return ret;
}

usize
e_base64_get_dec_len (const char *encoded)
{
	usize len, ret, i;

	if (!encoded) return 0;

	len = strlen (encoded);
	ret = len / 4 * 3;

	for (i = len; i > 0; i--) {
		if (encoded[i - 1] == '=') ret -= 1;
		else break;
	}

	return ret;
}

void
e_base64_enc (const char *plain, char *encoded_out)
{
	usize i, j, len;
	u32 n;

	if (!encoded_out) return;
	if (!plain) {
		encoded_out[0] = 0;
		return;
	}

	len = strlen (plain);
	for (i = 0, j = 0; i < len; i += 3, j += 4) {
		n = (u32) plain[i];
		n = i + 1 < len ? n << 8 | (u32) plain[i + 1] : n << 8;
		n = i + 2 < len ? n << 8 | (u32) plain[i + 2] : n << 8;

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

bool
e_base64_dec (const char *encoded, char *plain_out)
{
	usize i, j, len;
	u32 n;

	if (!plain_out) return false;
	if (!encoded) {
		plain_out[0] = 0;
		return true;
	}

	len = strlen (encoded);
	if (len % 4 != 0) return false;

	for (i = 0, j = 0; i < len; i += 4, j += 3) {
		if (!base64_is_valid_char(encoded[i])) return false;

		n = (u32) base64_dec_lut[(u8) encoded[i]];
		n = (n << 6) | (u32) base64_dec_lut[(u8) encoded[i + 1]];
		n = encoded[i + 2] == '=' ? n << 6 : (n << 6) | (u32) base64_dec_lut[(u8) encoded[i + 2]];
		n = encoded[i + 3] == '=' ? n << 6 : (n << 6) | (u32) base64_dec_lut[(u8) encoded[i + 3]];

		plain_out[j] = (char) (n >> 16);
		if (encoded[i + 2] != '=') plain_out[j + 1] = (char) (n >> 8);
		if (encoded[i + 3] != '=') plain_out[j + 2] = (char) n;
	}
	plain_out[j] = 0;

	return true;
}

char *
e_base64_enc_alloc (const char *plain)
{
	char *ret;

	ret = e_alloc (char, e_base64_get_enc_len (plain) + 1);
	e_base64_enc (plain, ret);

	return ret;
}

char *
e_base64_dec_alloc (const char *encoded)
{
	bool success;
	char *ret;

	ret = e_alloc (char, e_base64_get_dec_len (encoded) + 1);
	success = e_base64_dec (encoded, ret);
	if (!success) {
		e_free (ret);
		ret = nullptr;
	}

	return ret;
}

static bool
base64_is_valid_char (char c)
{
	return isalnum (c) || c == '/' || c == '+' || c == '=';
}

#endif /* E_CONFIG_MODULE_ENC */
