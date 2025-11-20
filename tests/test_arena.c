#define E_ARENA_IMPL
#include <e_arena.h>

#include <e_test.h>

#include <stdint.h>

void
test_arena (void)
{
	struct e_arena arena;
	unsigned char buf[32];
	void *ptr;

	arena = e_arena_init (buf, sizeof (buf));

	ptr = e_arena_alloc (&arena, uint32_t, 1);
	e_test_assert_ptr_aligned ("e_arena_alloc align", ptr, 4);

	ptr = e_arena_alloc_zero (&arena, uint16_t, 1);
	e_test_assert_ptr_aligned ("e_arena_alloc_zero align", ptr, 2);

	ptr = e_arena_alloc_aligned (&arena, 8, 8);
	e_test_assert_ptr_aligned ("e_arena_alloc_aligned align", ptr, 8);

	ptr = e_arena_alloc_size (&arena, 64);
	e_test_assert_null ("e_arena_alloc too large", ptr);
}
