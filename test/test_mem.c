#define E_MEM_IMPL
#include <e_mem.h>

#include <e_test.h>
#include <e_types.h>

void
test_mem (void)
{
	u8 mem1[] = {42, 69, 13, 37};
	u8 mem2[] = {37, 13, 69, 42};
	u8 mem3[] = {69, 42, 37, 31, 137};
	u8 mem4[] = {137, 31, 37, 42, 69};

	e_mem_reverse_bytes (mem1, 4);
	e_test_assert ("e_mem_reverse_bytes even", e_mem_eq (mem1, mem2, uint8_t, 4));
	e_mem_reverse_bytes (mem3, 5);
	e_test_assert ("e_mem_reverse_bytes odd", e_mem_eq (mem3, mem4, uint8_t, 5));
}
