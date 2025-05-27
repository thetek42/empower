#define E_COBS_IMPL
#include <e_cobs.h>

#include <e_test.h>
#include <e_types.h>

typedef struct {
	const char *plain;
	const char *enc;
	size_t plain_len;
	size_t enc_len;
} Case;

static const Case cases[] = {
	{
		.plain = "",
		.plain_len = 0,
		.enc = "\x01",
		.enc_len = 1,
	},
	{
		.plain = "1",
		.plain_len = 1,
		.enc = "\x02" "1",
		.enc_len = 2,
	},
	{
		.plain = "12345",
		.plain_len = 5,
		.enc = "\x06" "12345",
		.enc_len = 6,
	},
	{
		.plain = "12345" "\0" "6789",
		.plain_len = 10,
		.enc = "\x06" "12345" "\x05" "6789",
		.enc_len = 11,
	},
	{
		.plain = "\0" "12345" "\0" "6789",
		.plain_len = 11,
		.enc = "\x01" "\x06" "12345" "\x05" "6789",
		.enc_len = 12,
	},
	{
		.plain = "12345" "\0" "6789" "\0",
		.plain_len = 11,
		.enc = "\x06" "12345" "\x05" "6789" "\x01",
		.enc_len = 12,
	},
	{
		.plain = "\0",
		.plain_len = 1,
		.enc = "\x01" "\x01",
		.enc_len = 2,
	},
	{
		.plain = "\0" "\0",
		.plain_len = 2,
		.enc = "\x01" "\x01" "\x01",
		.enc_len = 3,
	},
	{
		.plain = "\0" "\0" "\0",
		.plain_len = 3,
		.enc = "\x01" "\x01" "\x01" "\x01",
		.enc_len = 4,
	},
	{
		.plain = "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123",
		.plain_len = 253,
		.enc = "\xFE" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123",
		.enc_len = 254,
	},
	{
		.plain = "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234",
		.plain_len = 254,
		.enc = "\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234",
		.enc_len = 255,
	},
	{
		.plain = "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345",
		.plain_len = 255,
		.enc = "\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x02" "5",
		.enc_len = 257,
	},
	{
		.plain = "\0" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345",
		.plain_len = 256,
		.enc = "\x01" "\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x02" "5",
		.enc_len = 258,
	},
	{
		.plain = "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\0",
		.plain_len = 254,
		.enc = "\xFE" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst123" "\x01",
		.enc_len = 255,
	},
	{
		.plain = "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\0",
		.plain_len = 255,
		.enc = "\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x01" "\x01",
		.enc_len = 257,
	},
	{
		.plain = "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345" "\0",
		.plain_len = 256,
		.enc = "\xFF" "0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst0123456789ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst1234" "\x02" "5" "\x01",
		.enc_len = 258,
	},
};

void
test_cobs (void)
{
	size_t len, i;
	char *out;
	char msg[64];

	for (i = 0; i < sizeof (cases) / sizeof (*cases); i += 1) {
		out = (char *) e_cobs_dec_alloc ((u8 *) cases[i].enc, cases[i].enc_len, &len);
		snprintf (msg, 64, "e_cobs_dec %zu len", i);
		e_test_assert_eq (msg, usize, len, cases[i].plain_len);
		snprintf (msg, 64, "e_cobs_dec %zu ptr", i);
		e_test_assert_mem_eq (msg, out, cases[i].plain, cases[i].plain_len);
		free (out);
	}

	for (i = 0; i < sizeof (cases) / sizeof (*cases); i += 1) {
		out = (char *) e_cobs_enc_alloc ((u8 *) cases[i].plain, cases[i].plain_len, &len);
		snprintf (msg, 64, "e_cobs_enc %zu len", i);
		e_test_assert_eq (msg, usize, len, cases[i].enc_len);
		snprintf (msg, 64, "e_cobs_enc %zu ptr", i);
		e_test_assert_mem_eq (msg, out, cases[i].enc, cases[i].enc_len);
		free (out);
	}
}
