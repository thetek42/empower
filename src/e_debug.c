#include "empower.h"

#if E_CONFIG_MODULE_DEBUG

void
e_debug_hexdump (const void *ptr, usize len)
{
	const u8 *p = ptr;
	usize i, j, k, l;
	char buf[70] = {0};
	u8 c;

	fprintf (stderr, "\n---=== hexdump %p ===---\n\n", (void *) p);

	if (p == nullptr) {
		fprintf (stderr, "nullptr\n\n");
		return;
	}

	if (len == 0) {
		fprintf (stderr, "len = 0\n\n");
		return;
	}

	for (i = 0; i < len; i += 16) {
		memset (buf, ' ', 69);
		snprintf (buf, 9, "%08" PRIx32, (u32) i);
		buf[8] = ':';
		for (j = 0; j < 16; j += 1) {
			if (i + j >= len) break;
			c = p[i + j];
			k = 10 + j * 2 + j / 2;
			l = 51 + j + j / 8;
			buf[k] = (char) (c / 16 >= 10 ? c / 16 + 87 : c / 16 + 48);
			buf[k + 1] = (char) (c % 16 >= 10 ? c % 16 + 87 : c % 16 + 48);
			buf[l] = 32 <= c && c <= 126 ? c : '.';
		}
		fprintf (stderr, "%s\n", buf);
	}
	fputc ('\n', stderr);
}

#endif /* E_CONFIG_MODULE_DEBUG */
