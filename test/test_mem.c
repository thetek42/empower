#define E_MEM_IMPL
#include <e_mem.h>

#include <e_test.h>
#include <e_types.h>

void
test_mem (void)
{
	u8 mem1[] = {42, 69, 13, 37};
	u8 mem2[] = {37, 13, 69, 42};
	u8 mem3[] = {42, 69, 13, 37, 137};
	u8 mem4[] = {137, 37, 13, 69, 42};
	u8 mem5[] = {0, 0, 0, 0};
	char *orig = "Hello, World!";
	char *copy;

	e_test_assert ("e_mem_is_aligned_to 1", e_mem_is_aligned_to ((void *) 0x28734584, 4));
	e_test_assert ("e_mem_is_aligned_to 2", !e_mem_is_aligned_to ((void *) 0x28734582, 4));
	e_test_assert ("e_mem_eq 1", e_mem_eq (mem1, mem3, u8, 4));
	e_test_assert ("e_mem_eq 2", !e_mem_eq (mem1, mem2, u8, 4));
	e_test_assert ("e_mem_is_zero 1", !e_mem_is_zero (mem1, u8, 4));
	e_test_assert ("e_mem_is_zero 2", e_mem_is_zero (mem5, u8, 4));
	e_mem_swap (mem1, mem2, u8, 4);
	e_test_assert ("e_mem_swap 1", e_mem_eq (mem1, &mem4[1], u8, 4));
	e_test_assert ("e_mem_swap 2", e_mem_eq (mem2, mem3, u8, 4));

	e_mem_reverse_bytes (mem1, 4);
	e_test_assert ("e_mem_reverse_bytes even", e_mem_eq (mem1, mem2, uint8_t, 4));
	e_mem_reverse_bytes (mem3, 5);
	e_test_assert ("e_mem_reverse_bytes odd", e_mem_eq (mem3, mem4, uint8_t, 5));

	copy = e_mem_strdup (orig);
	e_test_assert_str_eq ("e_mem_strdup", orig, copy);
	free (copy);
}
