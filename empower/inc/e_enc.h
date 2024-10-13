#ifndef _EMPOWER_ENC_H_
#define _EMPOWER_ENC_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_ENC

/*! e_enc *********************************************************************
 * 
 * This module provides functions for encoding and decoding as well as
 * encrypting and decrypting data.
 *
 * Module dependencies:
 *  - e_alloc (optional)
 *  - e_debug (transitive; optional)
 *  - e_log (transitive; optional)
 *
 * Example:
 *  | char *plain = "Hello, World!\n";
 *  | char *encoded = e_base64_enc_alloc (plain);
 *  | if (!encoded) return E_FAIL;
 *  | e_log_debug ("Encoded: %s", encoded);
 *  | e_free (encoded);
 *
 ******************************************************************************/

#include "convey.h"
#include "e_types.h"
#include "e_alloc.h"

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
