#define E_BITVEC_IMPL
#include <e_bitvec.h>

#include <e_test.h>

#include <stddef.h>

void
test_bitvec (void)
{
	struct e_bitvec bv;
	unsigned char data[8];
	size_t i;

	bv = e_bitvec_init (data, 64);

	e_bitvec_set (&bv, 19);
	e_bitvec_set (&bv, 21);
	e_test_assert_eq ("e_bitvec_set", unsigned char, data[2], 0x28); /* 0b00101000 */

	e_bitvec_negate (&bv, 23);
	e_bitvec_negate (&bv, 21);
	e_test_assert_eq ("e_bitvec_negate", unsigned char, data[2], 0x88); /* 0b10001000 */

	e_bitvec_unset (&bv, 23);
	e_test_assert_eq ("e_bitvec_unset", unsigned char, data[2], 0x8); /* 0b00001000 */

	e_bitvec_put (&bv, 22, 1);
	e_bitvec_put (&bv, 19, 0);
	e_test_assert_eq ("e_bitvec_put", unsigned char, data[2], 0x40); /* 0b01000000 */

	for (i = 35; i < 55; i++) {
		e_bitvec_set (&bv, i);
	}
	e_test_assert ("e_bitvec_all 1", e_bitvec_all (&bv, 35, 55));
	e_test_assert ("e_bitvec_all 2", !e_bitvec_all (&bv, 35, 56));
	e_test_assert ("e_bitvec_all 3", !e_bitvec_all (&bv, 34, 55));
	e_test_assert ("e_bitvec_all 4", e_bitvec_all (&bv, 4, 4));
	e_test_assert ("e_bitvec_any 1", e_bitvec_any (&bv, 25, 36));
	e_test_assert ("e_bitvec_any 2", !e_bitvec_any (&bv, 25, 35));
	e_test_assert ("e_bitvec_any 3", e_bitvec_any (&bv, 54, 60));
	e_test_assert ("e_bitvec_any 4", !e_bitvec_any (&bv, 55, 60));
	e_test_assert ("e_bitvec_any 5", !e_bitvec_any (&bv, 4, 4));
}
