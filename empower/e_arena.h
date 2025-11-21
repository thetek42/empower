#ifndef EMPOWER_ARENA_H_
#define EMPOWER_ARENA_H_

/******************************************************************************
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

#include <stddef.h>

#if !defined (E_ALIGNOF)
# if __STDC_VERSION__ >= 202311L
#  define E_ALIGNOF(expr) alignof (expr)
# elif __STDC_VERSION__ >= 201112L
#  define E_ALIGNOF(expr) _Alignof (expr)
# elif defined (__GNUC__) || defined (__clang__) || defined (__TINYC__)
#  define E_ALIGNOF(expr) __alignof__ (expr)
# elif defined (_MSC_VER)
#  define E_ALIGNOF(expr) __alignof (expr)
# else
#  define E_ALIGNOF(type) ((char *) (&((struct { char c; type t; } *) 0)->t) - (char *) 0)
# endif
#endif

#ifndef E_ALIGN_MAX
# if __STDC_VERSION__ >= 201112L
#  define E_ALIGN_MAX E_ALIGNOF (max_align_t)
# elif defined (__GNUC__) || defined (__clang__)
#   define E_ALIGN_MAX __BIGGEST_ALIGNMENT__
# elif __STDC_VERSION__ >= 199901L
#  include <stdint.h>
#  define E_ALIGN_MAX E_ALIGNOF (uint64_t)
# else
#  define E_ALIGN_MAX 8
# endif
#endif

#define e_arena_alloc(arena, T, nmemb) \
        ((T *) e_arena_alloc_aligned ((arena), sizeof (T) * (nmemb), E_ALIGNOF (T)))
#define e_arena_alloc_zero(arena, T, nmemb) \
        ((T *) e_arena_alloc_zero_aligned ((arena), sizeof (T) * (nmemb), E_ALIGNOF (T)))
#define e_arena_alloc_size(arena, size) \
        e_arena_alloc_aligned ((arena), (size), E_ALIGN_MAX)
#define e_arena_alloc_zero_size(arena, size) \
        e_arena_alloc_zero_aligned ((arena), (size), E_ALIGN_MAX)

struct e_arena {
	unsigned char *buf;
	size_t offset;
	size_t cap;
};

struct e_arena e_arena_init (void *buf, size_t cap);
void *e_arena_alloc_aligned (struct e_arena *arena, size_t size, size_t align);
void *e_arena_alloc_zero_aligned (struct e_arena *arena, size_t size, size_t align);
size_t e_arena_allocated_byte_count (const struct e_arena *arena);
size_t e_arena_remaining_byte_count (const struct e_arena *arena);

/******************************************************************************/

#ifdef E_ARENA_IMPL

/**
 * Initialize an arena allocator.
 *
 * The arena will use the pre-existing buffer `buf` with capacity `cap`. The
 * user is responsible for allocating and freeing the memory that the arena
 * uses (if necessary).
 */
struct e_arena
e_arena_init (void *buf,
              size_t cap)
{
	struct e_arena arena;
	arena.buf = buf;
	arena.offset = 0;
	arena.cap = cap;
	return arena;
}

/**
 * Allocate `size` bytes of alignment `align` in the arena allocator `arena`.
 * If the allocation fails due to insufficient space, NULL is returned.
 */
void *
e_arena_alloc_aligned (struct e_arena *arena,
                       size_t size,
                       size_t align)
{
	size_t offset, alignment_mismatch;
	void *ptr;

	offset = arena->offset;
	alignment_mismatch = ((size_t) arena->buf + offset) % align;
	if (alignment_mismatch > 0) {
		offset += align - alignment_mismatch;
	}
	if (offset + size > arena->cap) {
		return NULL;
	}
	ptr = &arena->buf[offset]; /* NOLINT */
	arena->offset = offset + size;

	return ptr;
}

/**
 * Allocate `size` bytes of alignment `align` in the arena allocator `arena`
 * and zero out the allocated memory. If the allocation fails due to
 * insufficient space, NULL is returned.
 */
void *
e_arena_alloc_zero_aligned (struct e_arena *arena,
                            size_t size,
                            size_t align)
{
	unsigned char *ptr;
        size_t i;

        ptr = e_arena_alloc_aligned (arena, size, align);
        if (ptr == NULL) return NULL;
	for (i = 0; i < size; i++) ptr[i] = 0;
	return ptr;
}

/**
 * Returns the number of bytes that have been allocated in `arena`.
 */
size_t
e_arena_allocated_byte_count (const struct e_arena *arena)
{
	return arena->offset;
}

/**
 * Returns the number of remaining bytes that can still be allocated in `arena`.
 */
size_t
e_arena_remaining_byte_count (const struct e_arena *arena)
{
	if (arena->offset >= arena->cap) return 0;
	return arena->cap - arena->offset;
}

#endif /* E_ARENA_IMPL */

#endif /* EMPOWER_ARENA_H_ */
