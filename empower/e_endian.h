#ifndef EMPOWER_ENDIAN_H_
#define EMPOWER_ENDIAN_H_

/**************************************************************************************************
 *
 * Empower / e_endian.h - Public Domain - https://git.tjdev.de/thetek/empower
 *
 * Conversion between big and little endian representations.
 *
 **************************************************************************************************/

#include <stdint.h>

uint16_t e_endian_u16_from_be (const uint8_t bytes[2]);
uint16_t e_endian_u16_from_le (const uint8_t bytes[2]);
uint32_t e_endian_u32_from_be (const uint8_t bytes[4]);
uint32_t e_endian_u32_from_le (const uint8_t bytes[4]);
uint64_t e_endian_u64_from_be (const uint8_t bytes[8]);
uint64_t e_endian_u64_from_le (const uint8_t bytes[8]);

void e_endian_u16_to_be (uint8_t bytes[2], uint16_t n);
void e_endian_u16_to_le (uint8_t bytes[2], uint16_t n);
void e_endian_u32_to_be (uint8_t bytes[4], uint32_t n);
void e_endian_u32_to_le (uint8_t bytes[4], uint32_t n);
void e_endian_u64_to_be (uint8_t bytes[8], uint64_t n);
void e_endian_u64_to_le (uint8_t bytes[8], uint64_t n);

/**************************************************************************************************/

#ifdef E_ENDIAN_IMPL

uint16_t
e_endian_u16_from_be (const uint8_t bytes[2])
{
    return (uint16_t) ((((uint16_t) bytes[0]) << 8) | ((uint16_t) bytes[1]));
}

uint16_t
e_endian_u16_from_le (const uint8_t bytes[2])
{
    return (uint16_t) ((((uint16_t) bytes[1]) << 8) | ((uint16_t) bytes[0]));
}

uint32_t
e_endian_u32_from_be (const uint8_t bytes[4])
{
    return ((((uint32_t) bytes[0]) << 24) | (((uint32_t) bytes[1]) << 16) |
            (((uint32_t) bytes[2]) << 8) | ((uint32_t) bytes[3]));
}

uint32_t
e_endian_u32_from_le (const uint8_t bytes[4])
{
    return ((((uint32_t) bytes[3]) << 24) | (((uint32_t) bytes[2]) << 16) |
            (((uint32_t) bytes[1]) << 8) | ((uint32_t) bytes[0]));
}

uint64_t
e_endian_u64_from_be (const uint8_t bytes[8])
{
    return ((((uint64_t) bytes[0]) << 56) | (((uint64_t) bytes[1]) << 48) |
            (((uint64_t) bytes[2]) << 40) | (((uint64_t) bytes[3]) << 32) |
            (((uint64_t) bytes[4]) << 24) | (((uint64_t) bytes[5]) << 16) |
            (((uint64_t) bytes[6]) << 8) | ((uint64_t) bytes[7]));
}

uint64_t
e_endian_u64_from_le (const uint8_t bytes[8])
{
    return ((((uint64_t) bytes[7]) << 56) | (((uint64_t) bytes[6]) << 48) |
            (((uint64_t) bytes[5]) << 40) | (((uint64_t) bytes[4]) << 32) |
            (((uint64_t) bytes[3]) << 24) | (((uint64_t) bytes[2]) << 16) |
            (((uint64_t) bytes[1]) << 8) | ((uint64_t) bytes[0]));
}

void
e_endian_u16_to_be (uint8_t bytes[2], uint16_t n)
{
    bytes[0] = (uint8_t) (n >> 8);
    bytes[1] = (uint8_t) n;
}

void
e_endian_u16_to_le (uint8_t bytes[2], uint16_t n)
{
    bytes[1] = (uint8_t) (n >> 8);
    bytes[0] = (uint8_t) n;
}

void
e_endian_u32_to_be (uint8_t bytes[4], uint32_t n)
{
    bytes[0] = (uint8_t) (n >> 24);
    bytes[1] = (uint8_t) (n >> 16);
    bytes[2] = (uint8_t) (n >> 8);
    bytes[3] = (uint8_t) n;
}

void
e_endian_u32_to_le (uint8_t bytes[4], uint32_t n)
{
    bytes[3] = (uint8_t) (n >> 24);
    bytes[2] = (uint8_t) (n >> 16);
    bytes[1] = (uint8_t) (n >> 8);
    bytes[0] = (uint8_t) n;
}

void
e_endian_u64_to_be (uint8_t bytes[8], uint64_t n)
{
    bytes[0] = (uint8_t) (n >> 56);
    bytes[1] = (uint8_t) (n >> 48);
    bytes[2] = (uint8_t) (n >> 40);
    bytes[3] = (uint8_t) (n >> 32);
    bytes[4] = (uint8_t) (n >> 24);
    bytes[5] = (uint8_t) (n >> 16);
    bytes[6] = (uint8_t) (n >> 8);
    bytes[7] = (uint8_t) n;
}

void
e_endian_u64_to_le (uint8_t bytes[8], uint64_t n)
{
    bytes[7] = (uint8_t) (n >> 56);
    bytes[6] = (uint8_t) (n >> 48);
    bytes[5] = (uint8_t) (n >> 40);
    bytes[4] = (uint8_t) (n >> 32);
    bytes[3] = (uint8_t) (n >> 24);
    bytes[2] = (uint8_t) (n >> 16);
    bytes[1] = (uint8_t) (n >> 8);
    bytes[0] = (uint8_t) n;
}

#endif /* E_ENDIAN_IMPL */

#endif /* EMPOWER_ENDIAN_H_ */
