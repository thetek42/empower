#define E_COBSR_IMPL
#include <e_cobsr.h>

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
		"\x01",
		1,
		"\x02" "\x01",
		2,
	},
	{
		"\x02",
		1,
		"\x02",
		1,
	},
	{
		"\x03",
		1,
		"\x03",
		1,
	},
	{
		"\x7E",
		1,
		"\x7E",
		1,
	},
	{
		"\x7F",
		1,
		"\x7F",
		1,
	},
	{
		"\x80",
		1,
		"\x80",
		1,
	},
	{
		"\xD5",
		1,
		"\xD5",
		1,
	},
	{
		"\xFE",
		1,
		"\xFE",
		1,
	},
	{
		"\xFF",
		1,
		"\xFF",
		1,
	},
	{
		"a" "\x02",
		2,
		"\x03" "a" "\x02",
		3,
	},
	{
		"a" "\x03",
		2,
		"\x03" "a",
		2,
	},
	{
		"a" "\xFF",
		2,
		"\xFF" "a",
		2,
	},
	{
		"\x05" "\x04" "\x03" "\x02" "\x01",
		5,
		"\x06" "\x05" "\x04" "\x03" "\x02" "\x01",
		6,
	},
	{
		"12345",
		5,
		"51234",
		5,
	},
	{
		"12345" "\x00" "\x04" "\x03" "\x02" "\x01",
		10,
		"\x06" "12345" "\x05" "\x04" "\x03" "\x02" "\x01",
		11,
	},
	{
		"12345" "\x00" "6789",
		10,
		"\x06" "123459678",
		10,
	},
	{
		"\x00" "12345" "\x00" "6789",
		11,
		"\x01" "\x06" "123459678",
		11,
	},
	{
		"12345" "\x00" "6789" "\x00",
		11,
		"\x06" "12345" "\x05" "6789" "\x01",
		12,
	},
	{
		"\x00",
		1,
		"\x01" "\x01",
		2,
	},
	{
		"\x00" "\x00",
		2,
		"\x01" "\x01" "\x01",
		3,
	},
	{
		"\x00" "\x00" "\x00",
		2,
		"\x01" "\x01" "\x01" "\x01",
		3,
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
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345",
		256,
	},
	{
		"\x00" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345",
		255,
		"\x01" "\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345",
		256,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\xFE",
		254,
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\xFE",
		255,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\xFF",
		254,
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123",
		254,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\x00",
		254,
		"\xFE" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\x01",
		255,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x00",
		255,
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x01" "\x01",
		257,
	},
	{
		"0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345" "\x00",
		256,
		"\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x02" "5" "\x01",
		258,
	},
};

void
test_cobsr_decode (void)
{
        enum e_cobsr_result result;
        unsigned char buffer[300];
	size_t len = (size_t) -1;
	size_t i;
	char msg[64];

	for (i = 0; i < sizeof (cases) / sizeof (*cases); i += 1) {
		result = e_cobsr_decode ((const unsigned char *) cases[i].enc,
                                         cases[i].enc_len, buffer, &len);
		sprintf (msg, "e_cobs_decode %d ret", (int) i);
		e_test_assert_eq (msg, int, result, E_COBSR_OK);
		sprintf (msg, "e_cobs_decode %d len", (int) i);
		e_test_assert_eq (msg, size_t, len, cases[i].plain_len);
		sprintf (msg, "e_cobs_decode %d ptr", (int) i);
		e_test_assert_mem_eq (msg, buffer, (const unsigned char *) cases[i].plain, len);
	}
}

void
test_cobsr_encode (void)
{
        enum e_cobsr_result result;
        unsigned char buffer[300];
	size_t len = (size_t) -1;
	size_t i;
	char msg[64];

	for (i = 0; i < sizeof (cases) / sizeof (*cases); i += 1) {
                result = e_cobsr_encode ((const unsigned char *) cases[i].plain,
                                         cases[i].plain_len, buffer, &len);
		sprintf (msg, "e_cobs_encode %d ret", (int) i);
		e_test_assert_eq (msg, int, result, E_COBSR_OK);
		sprintf (msg, "e_cobs_encode %d len", (int) i);
		e_test_assert_eq (msg, size_t, len, cases[i].enc_len);
		sprintf (msg, "e_cobs_encode %d ptr", (int) i);
		e_test_assert_mem_eq (msg, buffer, (const unsigned char *) cases[i].enc, len);
	}

}

void
test_cobsr (void)
{
        test_cobsr_decode ();
        test_cobsr_encode ();
}
