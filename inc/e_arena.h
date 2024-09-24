#ifndef _EMPOWER_ARENA_H_
#define _EMPOWER_ARENA_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_ARENA

#if !E_CONFIG_MODULE_ALLOC
# error "module e_arena depends on: e_alloc"
#endif /* !E_CONFIG_MODULE_ALLOC */

/*! e_arena *******************************************************************
 * 
 * This module provides a simple arena allocator.
 *
 * Module dependencies:
 *  - e_alloc
 *  - e_log (transitive)
 *
 ******************************************************************************/

E_VEC_DECLARE (u8, __E_Vec_Byte, __e_vec_byte);

typedef __E_Vec_Byte E_Arena;

/**
 * Allocate \nmemb items of type \T in the arena allocator \arena. Returns a
 * `void *` pointer to the item. The pointer will be aligned to `alignof (T)`.
 * Only returns `nullptr` when \arena is `nullptr`.
 */
#define e_arena_alloc(arena, T, nmemb) __e_arena_alloc ((arena), sizeof (T) * (nmemb), alignof (T))

/**
 * Allocate \size bytes of memory in the arena allocator \arena. Returns a
 * `void *` pointer to the memory. The pointer will be aligned to
 * `alignof (max_alignt_t)`, meaning that it will be compatible with all types.
 * Only returns `nullptr` when \arena is `nullptr`.
 */
#define e_arena_alloc_size(arena, size) __e_arena_alloc ((arena), (size), alignof (max_align_t))

/**
 * Allocate \size bytes of memory in the arena allocator \arena. Returns a
 * `void *` pointer to the memory. The pointer will be aligned to \align bytes.
 * Only returns `nullptr` when \arena is `nullptr`.
 */
#define e_arena_alloc_aligned(arena, size, align) __e_arena_alloc ((arena), (size), (align))

/**
 * Allocate \nmemb items of type \T in the arena allocator \arena. Returns a
 * `void *` pointer to the item. The pointer will be aligned to `alignof (T)`.
 * Only returns `nullptr` when \arena is `nullptr`. The memory will be zeroed.
 */
#define e_arena_alloc_zero(arena, T, nmemb) __e_arena_alloc_zero ((arena), sizeof (T) * (nmemb), alignof (T))

/**
 * Allocate \size bytes of memory in the arena allocator \arena. Returns a
 * `void *` pointer to the memory. The pointer will be aligned to
 * `alignof (max_alignt_t)`, meaning that it will be compatible with all types.
 * Only returns `nullptr` when \arena is `nullptr`. The memory will be zeroed.
 */
#define e_arena_alloc_zero_size(arena, size) __e_arena_alloc_zero ((arena), (size), alignof (max_align_t))

/**
 * Allocate \size bytes of memory in the arena allocator \arena. Returns a
 * `void *` pointer to the memory. The pointer will be aligned to \align bytes.
 * Only returns `nullptr` when \arena is `nullptr`. The memory will be zeroed.
 */
#define e_arena_alloc_zero_aligned(arena, size, align) __e_arena_alloc_zero ((arena), (size), (align))

/**
 * Initialize an arena allocator.
 */
E_Arena e_arena_init (void);

/**
 * Initialize an arena allocator with a given capacity \cap.
 */
E_Arena e_arena_init_with_cap (usize cap);

/**
 * Deinitialize an arena allocator \arena and free its occupied memory.
 */
void e_arena_deinit (E_Arena *arena);

/**
 * Grow the memory inside the arena allocator \arena to at least \cap bytes.
 * This behaves the same way as `e_vec_grow`.
 */
void e_arena_grow (E_Arena *arena, usize cap);

void *__e_arena_alloc (E_Arena *arena, usize size, usize align);
void *__e_arena_alloc_zero (E_Arena *arena, usize size, usize align);

#endif /* E_CONFIG_MODULE_ARENA */
#endif /* _EMPOWER_ARENA_H_ */
