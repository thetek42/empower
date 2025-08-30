#ifndef _EMPOWER_ARENA_H_
#define _EMPOWER_ARENA_H_

/*! e_arena *******************************************************************
 * 
 * Arena allocation.
 *
 * Example:
 *  | void *buf = e_alloc_size (64);
 *  | E_Arena arena = e_arena_init (buf, 64);
 *  | char *zeroed = e_arena_alloc_zero (&arena, char, 8);
 *  | int *one_int = e_arena_alloc (&arena, int, 1);
 *  | u64 *correctly_aligned_u64 = e_arena_alloc (&arena, u64, 1);
 *  | do_stuff ();
 *  | e_free (buf);
 *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>

/* compatibility annoyances ***************************************************/

#if __STDC_VERSION__ >= 201112L
# include <stdalign.h>
# define E_ARENA_ALIGNOF(expr) alignof (expr)
# define E_ARENA_MAX_ALIGN alignof (max_align_t)
#else /* __STDC_VERSION__ >= 201112L */
# if defined (__GNUC__) || defined (__clang__)
#  define E_ARENA_ALIGNOF(expr) __alignof__ (expr)
#  define E_ARENA_MAX_ALIGN __BIGGEST_ALIGNMENT__
# elif defined (_MSC_VER) /* defined (__GNUC__) || defined (__clang__) */
#  define E_ARENA_ALIGNOF(expr) __alignof (expr)
#  define E_ARENA_MAX_ALIGN __alignof (uint64_t)
# else /* defined (_MSC_VER) */
#  define E_ARENA_ALIGNOF_NOT_SUPPORTED
#  define E_ARENA_MAX_ALIGN sizeof (uint64_t)
# endif /* defined (_MSC_VER) */
#endif /* __STDC_VERSION__ >= 201112L */

/* public interface ***********************************************************/

#ifndef E_ARENA_ALIGNOF_NOT_SUPPORTED
# define e_arena_alloc(arena, T, nmemb) e_arena_alloc_aligned ((arena), sizeof (T) * (nmemb), E_ARENA_ALIGNOF (T))
# define e_arena_alloc_zero(arena, T, nmemb) e_arena_alloc_zero_aligned ((arena), sizeof (T) * (nmemb), E_ARENA_ALIGNOF (T))
#endif /* E_ARENA_ALIGNOF_NOT_SUPPORTED */

#define e_arena_alloc_size(arena, size) e_arena_alloc_aligned ((arena), (size), E_ARENA_MAX_ALIGN)
#define e_arena_alloc_zero_size(arena, size) e_arena_alloc_zero_aligned ((arena), (size), E_ARENA_MAX_ALIGN)

typedef struct {
	unsigned char *buf;
	size_t offset;
	size_t cap;
} E_Arena;

E_Arena e_arena_init (void *buf, size_t cap);
void *e_arena_alloc_aligned (E_Arena *arena, size_t size, size_t align);
void *e_arena_alloc_zero_aligned (E_Arena *arena, size_t size, size_t align);
size_t e_arena_allocated_byte_count (const E_Arena *arena);
size_t e_arena_remaining_byte_count (const E_Arena *arena);

/* implementation *************************************************************/

#ifdef E_ARENA_IMPL

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Initialize an arena allocator from a pre-existing buffer \buf with capacity
 * \cap. The user is responsible for allocating and freeing the memory that the
 * arena uses (if necessary).
 */
E_Arena
e_arena_init (void *buf, size_t cap)
{
	assert (buf != NULL && "buffer for e_arena_init cannot be null");

	return (E_Arena) {
		.buf = buf,
		.offset = 0,
		.cap = cap,
	};
}

/**
 * Allocate \size bytes of alignment \align in the arena allocator \arena. If
 * the allocation fails, `nullptr` is returned.
 */
void *
e_arena_alloc_aligned (E_Arena *arena, size_t size, size_t align)
{
	void *ptr;

	if (!arena) return NULL;

	if (arena->offset % align > 0) {
		arena->offset += align - arena->offset % align;
	}
	if (arena->offset + size > arena->cap) {
		return NULL;
	}
	ptr = &arena->buf[arena->offset];
	arena->offset += size;

	return ptr;
}

/**
 * Allocate \size bytes of alignment \align in the arena allocator \arena and
 * zero out the allocated memory. If the allocation fails, an error message is
 * printed and `nullptr` is returned.
 */
void *
e_arena_alloc_zero_aligned (E_Arena *arena, size_t size, size_t align)
{
	void *ptr = e_arena_alloc_aligned (arena, size, align);
	if (ptr) memset (ptr, 0, size);
	return ptr;
}

/**
 * Returns the number of bytes that have been allocated in \arena.
 */
size_t
e_arena_allocated_byte_count (const E_Arena *arena)
{
	if (!arena) return 0;
	return arena->offset;
}

/**
 * Returns the number of remaining bytes that can still be allocated in \arena.
 */
size_t
e_arena_remaining_byte_count (const E_Arena *arena)
{
	if (!arena) return 0;
	if (arena->offset >= arena->cap) return 0;
	return arena->cap - arena->offset;
}

#endif /* E_ARENA_IMPL */

#endif /* _EMPOWER_ARENA_H_ */
