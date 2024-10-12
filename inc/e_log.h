#ifndef _EMPOWER_LOG_H_
#define _EMPOWER_LOG_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_LOG

/*! e_log *********************************************************************
 *
 * This module provides functions and macros for logging. There are four levels
 * for logging: debug, info, warn and error. Log messages are printed to
 * `stderr`. The same formatting syntax as `fprintf` is used. In addition, this
 * module provides the `e_die` macro, which can be used for terminating the
 * programme with an error message. Debug log messages are only shown in debug
 * mode (`NDEBUG` not defined).
 *
 * Configuration options:
 *  - `LOG_COLOURED`: The log will be coloured using ANSI escape codes.
 *  - `LOG_WITH_FILE_LINE`: Source file and line will be appended.
 *
 ******************************************************************************/

#if E_CONFIG_LOG_COLOURED
# define __E_LOG_COLOUR_RESET "\x1b[0m"
# define __E_LOG_COLOUR_RED "\x1b[31m"
# define __E_LOG_COLOUR_GREEN "\x1b[32m"
# define __E_LOG_COLOUR_YELLOW "\x1b[33m"
# define __E_LOG_COLOUR_MAGENTA "\x1b[35m"
# define __E_LOG_COLOUR_GREY "\x1b[90m"
#else /* E_CONFIG_LOG_COLOURED */
# define __E_LOG_COLOUR_RESET ""
# define __E_LOG_COLOUR_RED ""
# define __E_LOG_COLOUR_GREEN ""
# define __E_LOG_COLOUR_YELLOW ""
# define __E_LOG_COLOUR_MAGENTA ""
# define __E_LOG_COLOUR_GREY ""
#endif /* E_CONFIG_LOG_COLOURED */

#if E_CONFIG_LOG_WITH_FILE_LINE
# define __E_LOG_FILE_LINE " " __E_LOG_COLOUR_GREY "(" E_MACRO_FILE_LINE ")" __E_LOG_COLOUR_RESET
#else /* E_CONFIG_LOG_WITH_FILE_LINE */
# define __E_LOG_FILE_LINE ""
#endif /* E_CONFIG_LOG_WITH_FILE_LINE */

/* TODO when clang finally supports __VA_OPT__ properly, enable it for clang */
#if E_STDC_VERSION >= E_STDC_VERSION_C23 && defined (E_COMPILER_GCC)

# ifdef NDEBUG
#  define e_log_debug(fmt, ...)
# else /* NDEBUG */
#  define e_log_debug(fmt, ...) fprintf (stderr, __E_LOG_COLOUR_MAGENTA "DEBUG " __E_LOG_COLOUR_RESET fmt __E_LOG_FILE_LINE "\n" __VA_OPT__ (,) __VA_ARGS__)
# endif /* NDEBUG */
# define e_log_info(fmt, ...)  fprintf (stderr, __E_LOG_COLOUR_GREEN   "INFO  " __E_LOG_COLOUR_RESET fmt __E_LOG_FILE_LINE "\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_warn(fmt, ...)  fprintf (stderr, __E_LOG_COLOUR_YELLOW  "WARN  " __E_LOG_COLOUR_RESET fmt __E_LOG_FILE_LINE "\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_error(fmt, ...) fprintf (stderr, __E_LOG_COLOUR_RED     "ERROR " __E_LOG_COLOUR_RESET fmt __E_LOG_FILE_LINE "\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_die(fmt, ...)                                                       \
	do {                                                                   \
		e_log_error (fmt __VA_OPT__ (,) __VA_ARGS__);                  \
		exit (EXIT_FAILURE);                                           \
	} while (0)

#else /* E_STDC_VERSION >= E_STDC_VERSION_C23 && defined (E_COMPILER_GCC) */

# ifdef NDEBUG
#  define e_log_debug(...)
# else /* NDEBUG */
#  define e_log_debug(...) __e_log_impl ( __E_LOG_FILE_LINE "\n", __E_LOG_COLOUR_MAGENTA "DEBUG " __E_LOG_COLOUR_RESET __VA_ARGS__)
# endif /* NDEBUG */
# define e_log_info(...)  __e_log_impl ( __E_LOG_FILE_LINE "\n", __E_LOG_COLOUR_GREEN   "INFO  " __E_LOG_COLOUR_RESET __VA_ARGS__)
# define e_log_warn(...)  __e_log_impl ( __E_LOG_FILE_LINE "\n", __E_LOG_COLOUR_YELLOW  "WARN  " __E_LOG_COLOUR_RESET __VA_ARGS__)
# define e_log_error(...) __e_log_impl ( __E_LOG_FILE_LINE "\n", __E_LOG_COLOUR_RED     "ERROR " __E_LOG_COLOUR_RESET __VA_ARGS__)
# define e_die(...)                                                            \
	do {                                                                   \
		e_log_error (__VA_ARGS__);                                     \
		exit (EXIT_FAILURE);                                           \
	} while (0)

E_ATTR_FORMAT (printf, 2, 3) void __e_log_impl (const char *file_pos, const char *fmt, ...);

#endif /* E_STDC_VERSION >= E_STDC_VERSION_C23 && defined (E_COMPILER_GCC) */

#endif /* E_CONFIG_MODULE_LOG */
#endif /* _EMPOWER_LOG_H_ */
