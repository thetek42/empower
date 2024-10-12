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
 *  - e_debug (optional; transitive)
 *  - e_log (optional; transient)
 *
 * Example:
 *  | char *plain = "Hello, World!\n";
 *  | char *encoded = e_base64_enc_alloc (plain);
 *  | if (!encoded) return E_FAIL;
 *  | e_log_debug ("Encoded: %s", encoded);
 *  | e_free (encoded);
 *
 ******************************************************************************/

usize e_base64_get_enc_len (const char *plain);
usize e_base64_get_dec_len (const char *encoded);
void e_base64_enc (const char *plain, char *encoded_out);
bool e_base64_dec (const char *encoded, char *plain_out);

#if E_CONFIG_MODULE_ALLOC
char *e_base64_enc_alloc (const char *plain);
char *e_base64_dec_alloc (const char *encoded);
#endif /* E_CONFIG_MODULE_ALLOC */

#endif /* E_CONFIG_MODULE_ENC */
#endif /* _EMPOWER_ENC_H_ */
