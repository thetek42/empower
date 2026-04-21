#ifndef E_BCD_H_
#define E_BCD_H_

/**************************************************************************************************
 *
 * Packed binary-coded decimals conversion.
 *
 * The standard "8421" encoding is used (i.e. tetrade values of 0-9 are valid, 10-15 are invalid).
 * Both little-endian (low digit first) and big-endian (high digit first) versions are provided.
 *
 * The decoding functions `e_bcd_dec_<int>_<le|be>` take the BCD bytes via the parameter `bcd`.
 * If `bcd` has a valid BCD number, the parsed version be written to `out` and `true` will be
 * returned. If `bcd` contains invalid decimals, `false` is returned.
 *
 * The encoding functions `e_bcd_dec_<int>_<le|be>` functions work in a similar manner. They take
 * the number that should be encoded via the parameter `num`. If the number can be encoded, the BCD
 * representation is written to `bcd` and `true` is returned. If the number cannot be encoded,
 * `false` is returned.
 *
 * TODO: signed!
 * TODO: c89
 *
 **************************************************************************************************/

#include <stdbool.h>
#include <stdint.h>

bool e_bcd_dec_u8_le (const uint8_t bcd[1], uint8_t *out);
bool e_bcd_dec_u8_be (const uint8_t bcd[1], uint8_t *out);
bool e_bcd_dec_u16_le (const uint8_t bcd[2], uint16_t *out);
bool e_bcd_dec_u16_be (const uint8_t bcd[2], uint16_t *out);
bool e_bcd_dec_u32_le (const uint8_t bcd[4], uint32_t *out);
bool e_bcd_dec_u32_be (const uint8_t bcd[4], uint32_t *out);
bool e_bcd_dec_u64_le (const uint8_t bcd[8], uint64_t *out);
bool e_bcd_dec_u64_be (const uint8_t bcd[8], uint64_t *out);

bool e_bcd_enc_u8_le (uint8_t bcd[1], uint8_t num);
bool e_bcd_enc_u8_be (uint8_t bcd[1], uint8_t num);
bool e_bcd_enc_u16_le (uint8_t bcd[2], uint16_t num);
bool e_bcd_enc_u16_be (uint8_t bcd[2], uint16_t num);
bool e_bcd_enc_u32_le (uint8_t bcd[4], uint32_t num);
bool e_bcd_enc_u32_be (uint8_t bcd[4], uint32_t num);
bool e_bcd_enc_u64_le (uint8_t bcd[8], uint64_t num);
bool e_bcd_enc_u64_be (uint8_t bcd[8], uint64_t num);

/**************************************************************************************************/

#ifdef E_BCD_IMPL

bool
e_bcd_dec_u8_le (const uint8_t bcd[1], uint8_t *out)
{
    uint8_t l = (bcd[0] >> 4) & 0x0F;
    uint8_t h = bcd[0] & 0x0F;
    if (l > 9 || h > 9) return false;
    *out = (uint8_t) (l + (h * 10));
    return true;
}

bool
e_bcd_dec_u8_be (const uint8_t bcd[1], uint8_t *out)
{
    uint8_t l = bcd[0] & 0x0F;
    uint8_t h = (bcd[0] >> 4) & 0x0F;
    if (l > 9 || h > 9) return false;
    *out = (uint8_t) (l + (h * 10));
    return true;
}

bool
e_bcd_dec_u16_le (const uint8_t bcd[2], uint16_t *out)
{
    uint8_t l, h;
    if (!e_bcd_dec_u8_le (&bcd[0], &l)) return false;
    if (!e_bcd_dec_u8_le (&bcd[1], &h)) return false;
    *out = (uint16_t) (((uint16_t) h * 100) + (uint16_t) l);
    return true;
}

bool
e_bcd_dec_u16_be (const uint8_t bcd[2], uint16_t *out)
{
    uint8_t l, h;
    if (!e_bcd_dec_u8_be (&bcd[0], &h)) return false;
    if (!e_bcd_dec_u8_be (&bcd[1], &l)) return false;
    *out = (uint16_t) (((uint16_t) h * 100) + (uint16_t) l);
    return true;
}

bool
e_bcd_dec_u32_le (const uint8_t bcd[4], uint32_t *out)
{
    uint16_t l, h;
    if (!e_bcd_dec_u16_le (&bcd[0], &l)) return false;
    if (!e_bcd_dec_u16_le (&bcd[2], &h)) return false;
    *out = ((uint32_t) h * 10000) + (uint32_t) l;
    return true;
}

bool
e_bcd_dec_u32_be (const uint8_t bcd[4], uint32_t *out)
{
    uint16_t l, h;
    if (!e_bcd_dec_u16_be (&bcd[0], &h)) return false;
    if (!e_bcd_dec_u16_be (&bcd[2], &l)) return false;
    *out = ((uint32_t) h * 10000) + (uint32_t) l;
    return true;
}

bool
e_bcd_dec_u64_le (const uint8_t bcd[8], uint64_t *out)
{
    uint32_t l, h;
    if (!e_bcd_dec_u32_le (&bcd[0], &l)) return false;
    if (!e_bcd_dec_u32_le (&bcd[4], &h)) return false;
    *out = ((uint64_t) h * 100000000) + (uint64_t) l;
    return true;
}

bool
e_bcd_dec_u64_be (const uint8_t bcd[8], uint64_t *out)
{
    uint32_t l, h;
    if (!e_bcd_dec_u32_be (&bcd[0], &h)) return false;
    if (!e_bcd_dec_u32_be (&bcd[4], &l)) return false;
    *out = ((uint64_t) h * 100000000) + (uint64_t) l;
    return true;
}

bool
e_bcd_enc_u8_le (uint8_t bcd[1], uint8_t num)
{
    if (num > 99) return false;
    bcd[0] = (uint8_t) ((num % 10) << 4);
    bcd[0] |= (uint8_t) (num / 10);
    return true;
}

bool
e_bcd_enc_u8_be (uint8_t bcd[1], uint8_t num)
{
    if (num > 99) return false;
    bcd[0] = (uint8_t) (num % 10);
    bcd[0] |= (uint8_t) ((num / 10) << 4);
    return true;
}

bool
e_bcd_enc_u16_le (uint8_t bcd[2], uint16_t num)
{
    if (num > 9999) return false;
    e_bcd_enc_u8_le (&bcd[0], (uint8_t) (num % 100));
    e_bcd_enc_u8_le (&bcd[1], (uint8_t) (num / 100));
    return true;
}

bool
e_bcd_enc_u16_be (uint8_t bcd[2], uint16_t num)
{
    if (num > 9999) return false;
    e_bcd_enc_u8_be (&bcd[0], (uint8_t) (num / 100));
    e_bcd_enc_u8_be (&bcd[1], (uint8_t) (num % 100));
    return true;
}

bool
e_bcd_enc_u32_le (uint8_t bcd[4], uint32_t num)
{
    if (num > 99999999) return false;
    e_bcd_enc_u16_le (&bcd[0], (uint16_t) (num % 10000));
    e_bcd_enc_u16_le (&bcd[2], (uint16_t) (num / 10000));
    return true;
}

bool
e_bcd_enc_u32_be (uint8_t bcd[4], uint32_t num)
{
    if (num > 99999999) return false;
    e_bcd_enc_u16_be (&bcd[0], (uint16_t) (num / 10000));
    e_bcd_enc_u16_be (&bcd[2], (uint16_t) (num % 10000));
    return true;
}

bool
e_bcd_enc_u64_le (uint8_t bcd[8], uint64_t num)
{
    if (num > 9999999999999999) return false;
    e_bcd_enc_u32_le (&bcd[0], (uint32_t) (num % 100000000));
    e_bcd_enc_u32_le (&bcd[4], (uint32_t) (num / 100000000));
    return true;
}

bool
e_bcd_enc_u64_be (uint8_t bcd[8], uint64_t num)
{
    if (num > 9999999999999999) return false;
    e_bcd_enc_u32_be (&bcd[0], (uint32_t) (num / 100000000));
    e_bcd_enc_u32_be (&bcd[4], (uint32_t) (num % 100000000));
    return true;
}

#endif /* E_BCD_IMPL */

#endif /* E_BCD_H_ */
