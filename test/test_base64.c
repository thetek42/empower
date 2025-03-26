#define E_BASE64_IMPL
#include <e_base64.h>

#include <stdlib.h>
#include <string.h>
#include <e_test.h>

void
test_base64 (void)
{
	char *plain = "Many hands make light work";
	char *invalid = "3bf7Dj2u4%73b";
	char *encoded, *plain_out;

	encoded = e_base64_enc_alloc (plain, strlen (plain));
	plain_out = e_base64_dec_alloc (encoded);

	e_test_assert_str_eq ("e_base64_enc", encoded, "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcms=");
	e_test_assert_str_eq ("e_base64_dec", plain_out, plain);
	e_test_assert_null ("e_base64_dec fail", e_base64_dec_alloc (invalid));

	free (plain_out);
	free (encoded);
}
