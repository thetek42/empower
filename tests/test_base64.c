#define E_BASE64_IMPL
#include <e_base64.h>

#include <e_test.h>

#include <string.h>

void
test_base64 (void)
{
	char *plain = "Many hands make light work";
	char *encoded = "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcms=";
	char *invalid = "3bf7Dj2u4%73b";
	char buffer[64];
	size_t len;
	int ok;

	len = e_base64_encode ((unsigned char *) plain, strlen (plain),
	                       (unsigned char *) buffer);
	e_test_assert_eq ("e_base64_encode len", size_t, len, strlen (encoded));
	e_test_assert_mem_eq ("e_base64_encode", buffer, encoded, len);

	ok = e_base64_decode ((unsigned char *) encoded, strlen (encoded),
	                      (unsigned char *) buffer, &len);
	e_test_assert ("e_base64_decode ret", ok);
	e_test_assert_eq ("e_base64_decode len", size_t, len, strlen (plain));
	e_test_assert_mem_eq ("e_base64_decode", buffer, plain, len);

	ok = e_base64_decode ((unsigned char *) invalid, strlen (invalid),
	                      (unsigned char *) buffer, &len);
	e_test_assert ("e_base64_decode invalid", !ok);
}
