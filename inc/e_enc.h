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
 *  - e_alloc
 *  - e_log (transient)
 *
 ******************************************************************************/

usize e_base64_get_enc_len (usize plain_len);
usize e_base64_get_dec_len (const char *encoded);
void e_base64_enc (const char *plain, char *encoded_out, usize plain_len);
bool e_base64_dec (const char *encoded, char *plain_out, usize encoded_len);
char *e_base64_enc_alloc (const char *plain, usize plain_len);
char *e_base64_dec_alloc (const char *encoded, usize encoded_len);

#endif /* E_CONFIG_MODULE_ENC */
#endif /* _EMPOWER_ENC_H_ */
