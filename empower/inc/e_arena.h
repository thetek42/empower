#ifndef _EMPOWER_ARENA_H_
#define _EMPOWER_ARENA_H_
#include "empower_config.h"
#if E_CONFIG_MODULE_ARENA

/*! e_arena *******************************************************************
 * 
 * This module provides a simple arena allocator. Similar to `e_alloc`, the
 * allocation functions never return `nullptr`.
 *
 * Module dependencies:
 *  - e_alloc
 *  - e_debug (transitive)
 *  - e_log (transitive, optional)
 *
 * Example:
 *  | E_Arena = e_arena_init (64);                  // initialize arena and allocate 64 bytes for it
 *  | u32 *x = e_arena_alloc (&arena, u32, 4);      // alloc four `u32`, aligned to 4 bytes
 *  | u8 *y = e_arena_alloc_zero_size (&arena, 8);  // alloc 8 bytes, aligned to `alignof (max_align_t)`, memory zeroed
 *  | u8 *z = e_arena_alloc_aligned (&arena, 1, 4); // alloc 1 byte, aligned to 4 bytes
 *  | e_arena_deinit (&arena);                      // free all the memory at once
 *
 ******************************************************************************/

#if !E_CONFIG_MODULE_ALLOC
# error "module e_arena depends on: e_alloc"
#endif /* !E_CONFIG_MODULE_ALLOC */

#include "convey.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_vec.h"

E_VEC_DECLARE (u8, __E_Vec_Byte, __e_vec_byte);

/**
 * The struct for the arena allocator. Refer to the module documentation.
 */
typedef __E_Vec_Byte E_Arena;

#ifndef C_ALIGNOF_NOT_SUPPORTED
# define e_arena_alloc(arena, T, nmemb) __e_arena_alloc ((arena), sizeof (T) * (nmemb), alignof (T))
# define e_arena_alloc_zero(arena, T, nmemb) __e_arena_alloc_zero ((arena), sizeof (T) * (nmemb), alignof (T))
#endif /* C_ALIGNOF_NOT_SUPPORTED */

#define e_arena_alloc_size(arena, size) __e_arena_alloc ((arena), (size), E_MAX_ALIGN)
#define e_arena_alloc_aligned(arena, size, align) __e_arena_alloc ((arena), (size), (align))
#define e_arena_alloc_zero_size(arena, size) __e_arena_alloc_zero ((arena), (size), E_MAX_ALIGN)
#define e_arena_alloc_zero_aligned(arena, size, align) __e_arena_alloc_zero ((arena), (size), (align))

E_Arena e_arena_init (usize cap);
void e_arena_deinit (E_Arena *arena);

void *__e_arena_alloc (E_Arena *arena, usize size, usize align);
void *__e_arena_alloc_zero (E_Arena *arena, usize size, usize align);

#endif /* E_CONFIG_MODULE_ARENA */
#endif /* _EMPOWER_ARENA_H_ */
