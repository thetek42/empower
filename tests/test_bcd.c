#include <string.h>
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L

# define E_BCD_IMPL
# include "e_bcd.h"
# include "e_test.h"

# include <stdint.h>

void
test_bcd (void)
{
    uint8_t bad[] = {0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F, 0x8F};
    uint8_t ubcd[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0x98, 0x76, 0x54};
    uint8_t bcd[8];
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;

    e_test_assert ("e_bcd_dec_u8_le ret", e_bcd_dec_u8_le (ubcd, &u8));
    e_test_assert_eq ("e_bcd_dec_u8_le", uint8_t, u8, 21);
    e_test_assert ("e_bcd_dec_u8_be ret", e_bcd_dec_u8_be (ubcd, &u8));
    e_test_assert_eq ("e_bcd_dec_u8_be", uint8_t, u8, 12);
    e_test_assert ("e_bcd_dec_u16_le ret", e_bcd_dec_u16_le (ubcd, &u16));
    e_test_assert_eq ("e_bcd_dec_u16_le", uint16_t, u16, 4321);
    e_test_assert ("e_bcd_dec_u16_be ret", e_bcd_dec_u16_be (ubcd, &u16));
    e_test_assert_eq ("e_bcd_dec_u16_be", uint16_t, u16, 1234);
    e_test_assert ("e_bcd_dec_u32_le ret", e_bcd_dec_u32_le (ubcd, &u32));
    e_test_assert_eq ("e_bcd_dec_u32_le", uint32_t, u32, 87654321);
    e_test_assert ("e_bcd_dec_u32_be ret", e_bcd_dec_u32_be (ubcd, &u32));
    e_test_assert_eq ("e_bcd_dec_u32_be", uint32_t, u32, 12345678);
    e_test_assert ("e_bcd_dec_u64_le ret", e_bcd_dec_u64_le (ubcd, &u64));
    e_test_assert_eq ("e_bcd_dec_u64_le", uint64_t, u64, 4567890987654321);
    e_test_assert ("e_bcd_dec_u64_be ret", e_bcd_dec_u64_be (ubcd, &u64));
    e_test_assert_eq ("e_bcd_dec_u64_be", uint64_t, u64, 1234567890987654);

    e_test_assert ("e_bcd_dec_u8_le bad", !e_bcd_dec_u8_le (bad, &u8));
    e_test_assert ("e_bcd_dec_u8_be bad", !e_bcd_dec_u8_be (bad, &u8));
    e_test_assert ("e_bcd_dec_u16_le bad", !e_bcd_dec_u16_le (bad, &u16));
    e_test_assert ("e_bcd_dec_u16_be bad", !e_bcd_dec_u16_be (bad, &u16));
    e_test_assert ("e_bcd_dec_u32_le bad", !e_bcd_dec_u32_le (bad, &u32));
    e_test_assert ("e_bcd_dec_u32_be bad", !e_bcd_dec_u32_be (bad, &u32));
    e_test_assert ("e_bcd_dec_u64_le bad", !e_bcd_dec_u64_le (bad, &u64));
    e_test_assert ("e_bcd_dec_u64_be bad", !e_bcd_dec_u64_be (bad, &u64));

    e_test_assert ("e_bcd_enc_u8_le ret", e_bcd_enc_u8_le (bcd, 21));
    e_test_assert_mem_eq ("e_bcd_enc_u8_le", bcd, ubcd, 1);
    e_test_assert ("e_bcd_enc_u8_be ret", e_bcd_enc_u8_be (bcd, 12));
    e_test_assert_mem_eq ("e_bcd_enc_u8_be", bcd, ubcd, 1);
    e_test_assert ("e_bcd_enc_u16_le ret", e_bcd_enc_u16_le (bcd, 4321));
    e_test_assert_mem_eq ("e_bcd_enc_u16_le", bcd, ubcd, 2);
    e_test_assert ("e_bcd_enc_u16_be ret", e_bcd_enc_u16_be (bcd, 1234));
    e_test_assert_mem_eq ("e_bcd_enc_u16_be", bcd, ubcd, 2);
    e_test_assert ("e_bcd_enc_u32_le ret", e_bcd_enc_u32_le (bcd, 87654321));
    e_test_assert_mem_eq ("e_bcd_enc_u32_le", bcd, ubcd, 4);
    e_test_assert ("e_bcd_enc_u32_be ret", e_bcd_enc_u32_be (bcd, 12345678));
    e_test_assert_mem_eq ("e_bcd_enc_u32_be", bcd, ubcd, 4);
    e_test_assert ("e_bcd_enc_u64_le ret", e_bcd_enc_u64_le (bcd, 4567890987654321));
    e_test_assert_mem_eq ("e_bcd_enc_u64_le", bcd, ubcd, 8);
    e_test_assert ("e_bcd_enc_u64_be ret", e_bcd_enc_u64_be (bcd, 1234567890987654));
    e_test_assert_mem_eq ("e_bcd_enc_u64_be", bcd, ubcd, 8);

    e_test_assert ("e_bcd_enc_u8_le bad", !e_bcd_enc_u8_le (bcd, 100));
    e_test_assert ("e_bcd_enc_u8_be bad", !e_bcd_enc_u8_be (bcd, 100));
    e_test_assert ("e_bcd_enc_u16_le bad", !e_bcd_enc_u16_le (bcd, 10000));
    e_test_assert ("e_bcd_enc_u16_be bad", !e_bcd_enc_u16_be (bcd, 10000));
    e_test_assert ("e_bcd_enc_u32_le bad", !e_bcd_enc_u32_le (bcd, 100000000));
    e_test_assert ("e_bcd_enc_u32_be bad", !e_bcd_enc_u32_be (bcd, 100000000));
    e_test_assert ("e_bcd_enc_u64_le bad", !e_bcd_enc_u64_le (bcd, 10000000000000000));
    e_test_assert ("e_bcd_enc_u64_be bad", !e_bcd_enc_u64_be (bcd, 10000000000000000));
}

#else /* defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

void
test_bcd (void)
{
}

#endif /* defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */
