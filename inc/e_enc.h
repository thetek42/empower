#ifndef _EMPOWER_ENC_H_
#define _EMPOWER_ENC_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_ENC

#if !E_CONFIG_MODULE_ALLOC || !E_CONFIG_MODULE_RESULT
# error "module e_enc depends on: e_alloc"
#endif /* !E_CONFIG_MODULE_ALLOC */

/*! e_enc *********************************************************************
 * 
 * This module provides functions for encoding and decoding as well as
 * encrypting and decrypting data.
 *
 * Module dependencies:
 *  - e_alloc (optional)
 *  - e_log (optional; transient)
 *
 ******************************************************************************/

/**
 * Get the length required to store the result of Base64-encoding a
 * nul-terminated plain text \plain. The returned length does not include the
 * terminating nul byte.
 */
usize e_base64_get_enc_len (const char *plain);

/**
 * Get the length required to store the result of Base64-decoding the
 * nul-terminated encoded text \encoded. The length of \encoded is determined
 * using `strlen`. The returned length does not include the terminating nul
 * byte.
 */
usize e_base64_get_dec_len (const char *encoded);

/**
 * Encode a nul-terminated plain text \plain using Base64 and store the result
 * in \encoded_out. \encoded_out must be capable of holding at least
 * `e_base64_get_enc_len (encoded) + 1` bytes (including the nul terminator).
 */
void e_base64_enc (const char *plain, char *encoded_out);

/**
 * Decode a nul-terminated Base64-encoded text \encoded and store the result in
 * \plain_out. \plain_out must be capable of holding at least
 * `e_base64_get_dec_len (encoded) + 1` bytes (including the nul terminator).
 * Returns true when the decoding was successful.
 */
bool e_base64_dec (const char *encoded, char *plain_out);

#if E_CONFIG_MODULE_ALLOC

/**
 * Encode a nul-terminated plain text \plain using Base64, allocates enough
 * memory for storing the result and stores the encoded text including a nul
 * terminator in there. A pointer to the allocated memory is returned. The
 * memory must be freed by the user using `e_free`.
 */
char *e_base64_enc_alloc (const char *plain);

/**
 * Decode a nul-terminated Base64-encoded text \encoded, allocates enough memory
 * for storing the result and stores the decoded text including a nul terminator
 * in there. A pointer to the allocated memory is returned. If the decoding
 * fails, `nullptr` is returned. A pointer to the allocated memory is returned.
 * The memory must be freed by the user using `e_free`.
 */
char *e_base64_dec_alloc (const char *encoded);

#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_ENC */
#endif /* _EMPOWER_ENC_H_ */
