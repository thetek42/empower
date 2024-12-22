#define E_BASE64_IMPL
#include <e_base64.h>

#include <stdlib.h>
#include <e_test.h>

void
test_base64 (void)
{
	char *plain = "Many hands make light work.";
	char *encoded, *plain_out;

	encoded = e_base64_enc_alloc (plain);
	plain_out = e_base64_dec_alloc (encoded);

	e_test_assert_str_eq ("e_base64_enc", encoded, "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");
	e_test_assert_str_eq ("e_base64_dec", plain_out, plain);

	free (encoded);
	free (plain_out);
}
