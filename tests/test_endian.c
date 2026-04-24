#if __STDC_VERSION__ >= 199901L

# define E_ENDIAN_IMPL
# include "e_endian.h"
# include "e_test.h"

# include <stdint.h>
# include <string.h>

void
test_endian (void)
{
    unsigned char endian[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

    e_test_assert_eq ("e_endian_u16_from_be", uint16_t, e_endian_u16_from_be (endian), 0x1234);
    e_test_assert_eq ("e_endian_u16_from_le", uint16_t, e_endian_u16_from_le (endian), 0x3412);
    e_test_assert_eq ("e_endian_u32_from_be", uint32_t, e_endian_u32_from_be (endian), 0x12345678);
    e_test_assert_eq ("e_endian_u32_from_le", uint32_t, e_endian_u32_from_le (endian), 0x78563412);
    e_test_assert_eq ("e_endian_u64_from_be", uint64_t, e_endian_u64_from_be (endian),
                      0x123456789ABCDEF0);
    e_test_assert_eq ("e_endian_u64_from_le", uint64_t, e_endian_u64_from_le (endian),
                      0xF0DEBC9A78563412);

    memset (endian, 0, sizeof (endian));
    e_endian_u16_to_be (endian, 0x1234);
    e_test_assert_eq ("e_endian_u16_to_be", uint16_t, e_endian_u16_from_be (endian), 0x1234);
    memset (endian, 0, sizeof (endian));
    e_endian_u16_to_le (endian, 0x1234);
    e_test_assert_eq ("e_endian_u16_to_le", uint16_t, e_endian_u16_from_le (endian), 0x1234);
    memset (endian, 0, sizeof (endian));
    e_endian_u32_to_be (endian, 0x12345678);
    e_test_assert_eq ("e_endian_u32_to_be", uint32_t, e_endian_u32_from_be (endian), 0x12345678);
    memset (endian, 0, sizeof (endian));
    e_endian_u32_to_le (endian, 0x12345678);
    e_test_assert_eq ("e_endian_u32_to_le", uint32_t, e_endian_u32_from_le (endian), 0x12345678);
    memset (endian, 0, sizeof (endian));
    e_endian_u64_to_be (endian, 0x123456789ABCDEF0);
    e_test_assert_eq ("e_endian_u64_to_be", uint64_t, e_endian_u64_from_be (endian),
                      0x123456789ABCDEF0);
    memset (endian, 0, sizeof (endian));
    e_endian_u64_to_le (endian, 0x123456789ABCDEF0);
    e_test_assert_eq ("e_endian_u64_to_le", uint64_t, e_endian_u64_from_le (endian),
                      0x123456789ABCDEF0);
}

#else /* __STDC_VERSION__ >= 199901L */

void
test_endian (void)
{
}

#endif /* __STDC_VERSION__ >= 199901L */
