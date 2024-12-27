#define E_ARENA_IMPL
#include <e_arena.h>

#include <e_mem.h>
#include <e_test.h>
#include <e_types.h>

void
test_arena (void)
{
	E_Arena arena;
	char buf[32];
	void *ptr;

	arena = e_arena_init (buf, sizeof (buf));

	ptr = e_arena_alloc (&arena, u32, 1);
	e_test_assert_ptr_eq ("e_arena_alloc ptr", ptr, arena.buf);
	e_test_assert_ptr_aligned ("e_arena_alloc align", ptr, 4);
	e_test_assert_eq ("e_arena_alloc len", usize, arena.offset, 4);

	ptr = e_arena_alloc_zero (&arena, u16, 1);
	e_test_assert_ptr_eq ("e_arena_alloc_zero ptr", ptr, arena.buf + 4);
	e_test_assert_ptr_aligned ("e_arena_alloc_zero align", ptr, 2);
	e_test_assert_eq ("e_arena_alloc_zero len", usize, arena.offset, 6);
	e_test_assert ("e_arena_alloc_zero mem", e_mem_is_zero_size (ptr, 2));

	ptr = e_arena_alloc_aligned (&arena, 8, 8);
	e_test_assert_ptr_eq ("e_arena_alloc_aligned ptr", ptr, arena.buf + 8);
	e_test_assert_ptr_aligned ("e_arena_alloc_aligned align", ptr, 8);
	e_test_assert_eq ("e_arena_alloc_aligned len", usize, arena.offset, 16);

	ptr = e_arena_alloc_size (&arena, 64);
	e_test_assert_null ("e_arena_alloc too large", ptr);
}
