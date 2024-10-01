#include "empower.h"

#if E_CONFIG_MODULE_ARENA

E_VEC_IMPLEMENT (u8, __E_Vec_Byte, __e_vec_byte);

E_Arena
e_arena_init (usize cap)
{
	return __e_vec_byte_init_with_cap (cap);
}

void
e_arena_deinit (E_Arena *arena)
{
	__e_vec_byte_deinit (arena);
}

void *
__e_arena_alloc (E_Arena *arena, usize size, usize align)
{
	void *ptr;

	if (!arena) return nullptr;

	if (arena->len % align > 0) {
		arena->len += align - arena->len % align;
	}
	if (arena->len + size > arena->cap) {
		e_die ("failed to alloc %zu bytes with alignment %zu in arena "
		       "of capacity %zu and %zu used bytes", size, align,
		       arena->cap, arena->len);
	}
	ptr = arena->ptr + arena->len;
	arena->len += size;

	return ptr;
}

void *
__e_arena_alloc_zero (E_Arena *arena, usize size, usize align)
{
	void *ptr;

	if (!arena) return nullptr;
	ptr = __e_arena_alloc (arena, size, align);
	memset (ptr, 0, size);

	return ptr;
}

#endif /* E_CONFIG_MODULE_ARENA */
