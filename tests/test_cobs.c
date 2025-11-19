#define E_COBS_IMPL
#include <e_cobs.h>

#include <e_test.h>

#include <stdio.h>

struct test_case {
	const char *plain;
	size_t plain_len;
	const char *enc;
	size_t enc_len;
};

static const struct test_case cases[] = {
	{
		"",
		0,
		"\x01",
		1,
	},
	{
		"1",
		1,
		"\x02" "1",
		2,
	},
	{
		"12345",
		5,
		"\x06" "12345",
		6,
	},
	{
		"12345" "\0" "6789",
		10,
		"\x06" "12345" "\x05" "6789",
		11,
	},
	{
		"\0" "12345" "\0" "6789",
		11,
		"\x01" "\x06" "12345" "\x05" "6789",
		12,
	},
	{
		"12345" "\0" "6789" "\0",
		11,
		"\x06" "12345" "\x05" "6789" "\x01",
		12,
	},
	{
		"\0",
		1,
		"\x01" "\x01",
		2,
	},
	{
		"\0" "\0",
		2,
		"\x01" "\x01" "\x01",
		3,
	},
	{
		"\0" "\0" "\0",
		3,
		"\x01" "\x01" "\x01" "\x01",
		4,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123",
		253,
		"\xFE" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123",
		254,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234",
		254,
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234",
		255,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345",
		255,
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x02" "5",
		257,
	},
	{
		"\0" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345",
		256,
		"\x01" "\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x02" "5",
		258,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\0",
		254,
		"\xFE" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\x01",
		255,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\0",
		255,
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x01" "\x01",
		257,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345" "\0",
		256,
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x02" "5" "\x01",
		258,
	},
};

void
test_cobs_decode (void)
{
        enum e_cobs_result result;
        unsigned char buffer[300];
	size_t len = (size_t) -1;
	size_t i;
	char msg[64];

	for (i = 0; i < sizeof (cases) / sizeof (*cases); i += 1) {
		result = e_cobs_decode ((const unsigned char *) cases[i].enc,
                                        cases[i].enc_len, buffer, &len);
		sprintf (msg, "e_cobs_decode %d ret", (int) i);
		e_test_assert_eq (msg, int, result, E_COBS_OK);
		sprintf (msg, "e_cobs_decode %d len", (int) i);
		e_test_assert_eq (msg, size_t, len, cases[i].plain_len);
		sprintf (msg, "e_cobs_decode %d ptr", (int) i);
		e_test_assert_mem_eq (msg, buffer, (const unsigned char *) cases[i].plain, len);
	}
}

void
test_cobs_encode (void)
{
        enum e_cobs_result result;
        unsigned char buffer[300];
	size_t len = (size_t) -1;
	size_t i;
	char msg[64];

	for (i = 0; i < sizeof (cases) / sizeof (*cases); i += 1) {
                result = e_cobs_encode ((const unsigned char *) cases[i].plain,
                                        cases[i].plain_len, buffer, &len);
		sprintf (msg, "e_cobs_encode %d ret", (int) i);
		e_test_assert_eq (msg, int, result, E_COBS_OK);
		sprintf (msg, "e_cobs_encode %d len", (int) i);
		e_test_assert_eq (msg, size_t, len, cases[i].enc_len);
		sprintf (msg, "e_cobs_encode %d ptr", (int) i);
		e_test_assert_mem_eq (msg, buffer, (const unsigned char *) cases[i].enc, len);
	}

}

void
test_cobs (void)
{
        test_cobs_decode ();
        test_cobs_encode ();
}
