#ifndef EMPOWER_H_
#define EMPOWER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/* c version checks ***********************************************************/

#if __STDC_VERSION__ >= 202000L
#define E_STDC23
#endif

#if __STDC_VERSION__ >= 201000L
#define E_STDC11
#endif

#if __STDC_VERSION__ >= 199900L
#define E_STDC99
#endif

#if __STDC_VERSION__ < 199900L
# error "empower does not support C89."
#endif

/* common types ***************************************************************/

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef signed char ichar;
typedef signed long long llong;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef size_t usize;
typedef ssize_t isize;
typedef intptr_t iptr;
typedef uintptr_t uptr;
typedef float f32;
typedef double f64;
typedef long double f128;

/* macro utils ****************************************************************/

#ifndef E_STDC23
# define nullptr NULL
#endif

#define E_MACRO_STRINGIFY(x) E_MACRO_STRINGIFY_HELPER (x)
#define E_MACRO_STRINGIFY_HELPER(x) #x

/* logging ********************************************************************/

#ifdef E_STDC23
# define e_log_debug(fmt, ...) fprintf (stderr, "\x1b[35mDEBUG \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_info(fmt, ...)  fprintf (stderr, "\x1b[32mINFO  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_warn(fmt, ...)  fprintf (stderr, "\x1b[33mWARN  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_error(fmt, ...) fprintf (stderr, "\x1b[31mERROR \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_die(fmt, ...)                                                       \
	do {                                                                   \
		e_log_error (fmt __VA_OPT__ (,) __VA_ARGS__);                  \
		exit (EXIT_FAILURE);                                           \
	} while (0)
#else // E_STDC23
# define e_log_debug(...) __e_log_impl (__E_LOG_LEVEL_DEBUG, " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
# define e_log_info(...)  __e_log_impl (__E_LOG_LEVEL_INFO,  " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
# define e_log_warn(...)  __e_log_impl (__E_LOG_LEVEL_WARN,  " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
# define e_log_error(...) __e_log_impl (__E_LOG_LEVEL_ERROR, " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
# define e_die(...)                                                            \
	do {                                                                   \
		e_log_error (__VA_ARGS__);                                     \
		exit (EXIT_FAILURE);                                           \
	} while (0)
enum __e_log_level {
	__E_LOG_LEVEL_DEBUG,
	__E_LOG_LEVEL_INFO,
	__E_LOG_LEVEL_WARN,
	__E_LOG_LEVEL_ERROR
};
void __e_log_impl (enum __e_log_level level, const char *formatted_file_pos, const char *fmt, ...);
#endif // E_STDC23

/* memory utilities ***********************************************************/

#define e_alloc(type, nmemb) __e_mem_alloc (sizeof (type) * (nmemb))
#define e_alloc_size(size) __e_mem_alloc ((size))
#define e_alloc_zero(type, nmemb) __e_mem_calloc ((nmemb), sizeof (type))
#define e_alloc_zero_size(size, nmemb) __e_mem_calloc ((nmemb), (size))
#define e_realloc(ptr, type, nmemb) __e_mem_realloc ((ptr), sizeof (type) * (nmemb))
#define e_realloc_size(ptr, size) __e_mem_realloc ((ptr), (size))
#define e_free(ptr) free ((ptr))
#define e_new(type) e_alloc (type, 1)
#define e_new_zero(type) e_alloc_zero (type, 1)

void *__e_mem_alloc (usize size);
void *__e_mem_calloc (usize nmemb, usize size);
void *__e_mem_realloc (void *ptr, usize size);

#endif // EMPOWER_H_
