#define E_MEM_IMPL
#include <e_mem.h>

#include <e_test.h>

#include <stdlib.h>
#include <string.h>
#if __STDC_VERSION__ >= 199901L
# include <stdint.h>
#endif

void
test_mem (void)
{
	unsigned char mem1[] = {42, 69, 13, 37};
	unsigned char mem2[] = {37, 13, 69, 42};
	unsigned char mem3[] = {42, 69, 13, 37, 137};
	unsigned char mem4[] = {137, 37, 13, 69, 42};
	unsigned char mem5[] = {0, 0, 0, 0};
	char *orig = "Hello, World!";
	char *copy;

	e_test_assert ("e_mem_is_aligned_to 1", e_mem_is_aligned_to ((void *) 0x28734584, 4));
	e_test_assert ("e_mem_is_aligned_to 2", !e_mem_is_aligned_to ((void *) 0x28734582, 4));
	e_test_assert ("e_mem_eq 1", e_mem_eq (mem1, mem3, unsigned char, 4));
	e_test_assert ("e_mem_eq 2", !e_mem_eq (mem1, mem2, unsigned char, 4));
	e_test_assert ("e_mem_is_zero 1", !e_mem_is_zero (mem1, unsigned char, 4));
	e_test_assert ("e_mem_is_zero 2", e_mem_is_zero (mem5, unsigned char, 4));
	e_mem_swap (mem1, mem2, unsigned char, 4);
	e_test_assert ("e_mem_swap 1", e_mem_eq (mem1, &mem4[1], unsigned char, 4));
	e_test_assert ("e_mem_swap 2", e_mem_eq (mem2, mem3, unsigned char, 4));

	e_mem_reverse_bytes (mem1, 4);
	e_test_assert ("e_mem_reverse_bytes even", e_mem_eq (mem1, mem2, unsigned char, 4));
	e_mem_reverse_bytes (mem3, 5);
	e_test_assert ("e_mem_reverse_bytes odd", e_mem_eq (mem3, mem4, unsigned char, 5));

	copy = e_mem_strdup (orig);
	e_test_assert_str_eq ("e_mem_strdup", orig, copy);
	free (copy);

	copy = e_mem_strdup_n ("Hello, World!", 99);
	e_test_assert_str_eq ("e_mem_strdup_n excess", orig, copy);
	free (copy);

	copy = e_mem_strdup_n ("Hello, World!foobarbaz", 13);
	e_test_assert_str_eq ("e_mem_strdup_n cutoff", orig, copy);
	free (copy);

#if __STDC_VERSION__ >= 199901L

	unsigned char endian[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

	e_test_assert_eq ("e_mem_read_u16_be", uint16_t, e_mem_read_u16_be (endian), 0x1234);
	e_test_assert_eq ("e_mem_read_u16_le", uint16_t, e_mem_read_u16_le (endian), 0x3412);
	e_test_assert_eq ("e_mem_read_u32_be", uint32_t, e_mem_read_u32_be (endian), 0x12345678);
	e_test_assert_eq ("e_mem_read_u32_le", uint32_t, e_mem_read_u32_le (endian), 0x78563412);
	e_test_assert_eq ("e_mem_read_u64_be", uint64_t, e_mem_read_u64_be (endian), 0x123456789ABCDEF0);
	e_test_assert_eq ("e_mem_read_u64_le", uint64_t, e_mem_read_u64_le (endian), 0xF0DEBC9A78563412);

	memset (endian, 0, sizeof (endian));
	e_mem_write_u16_be (endian, 0x1234);
	e_test_assert_eq ("e_mem_write_u16_be", uint16_t, e_mem_read_u16_be (endian), 0x1234);
	memset (endian, 0, sizeof (endian));
	e_mem_write_u16_le (endian, 0x1234);
	e_test_assert_eq ("e_mem_write_u16_le", uint16_t, e_mem_read_u16_le (endian), 0x1234);
	memset (endian, 0, sizeof (endian));
	e_mem_write_u32_be (endian, 0x12345678);
	e_test_assert_eq ("e_mem_write_u32_be", uint32_t, e_mem_read_u32_be (endian), 0x12345678);
	memset (endian, 0, sizeof (endian));
	e_mem_write_u32_le (endian, 0x12345678);
	e_test_assert_eq ("e_mem_write_u32_le", uint32_t, e_mem_read_u32_le (endian), 0x12345678);
	memset (endian, 0, sizeof (endian));
	e_mem_write_u64_be (endian, 0x123456789ABCDEF0);
	e_test_assert_eq ("e_mem_write_u64_be", uint64_t, e_mem_read_u64_be (endian), 0x123456789ABCDEF0);
	memset (endian, 0, sizeof (endian));
	e_mem_write_u64_le (endian, 0x123456789ABCDEF0);
	e_test_assert_eq ("e_mem_write_u64_le", uint64_t, e_mem_read_u64_le (endian), 0x123456789ABCDEF0);

#endif /* __STDC_VERSION__ >= 199901L */
}
