#ifndef _EMPOWER_LOG_H_
#define _EMPOWER_LOG_H_

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
 *  - `E_LOG_CONFIG_NO_COLOUR`: Disable ANSI escape code colouring.
 *  - `E_LOG_CONFIG_NO_FILE_LINE`: Disable source location in the log.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/* public interface ***********************************************************/

#if E_LOG_CONFIG_NO_COLOUR
# define __E_LOG_COLOUR_RESET ""
# define __E_LOG_COLOUR_RED ""
# define __E_LOG_COLOUR_GREEN ""
# define __E_LOG_COLOUR_YELLOW ""
# define __E_LOG_COLOUR_MAGENTA ""
# define __E_LOG_COLOUR_GREY ""
#else /* E_LOG_CONFIG_NO_COLOUR */
# define __E_LOG_COLOUR_RESET "\x1b[0m"
# define __E_LOG_COLOUR_RED "\x1b[31m"
# define __E_LOG_COLOUR_GREEN "\x1b[32m"
# define __E_LOG_COLOUR_YELLOW "\x1b[33m"
# define __E_LOG_COLOUR_MAGENTA "\x1b[35m"
# define __E_LOG_COLOUR_GREY "\x1b[90m"
#endif /* E_LOG_CONFIG_NO_COLOUR */

#if E_LOG_CONFIG_NO_FILE_LINE
# define __E_LOG_FILE_LINE ""
#else /* E_LOG_CONFIG_NO_FILE_LINE */
# define __E_LOG_STRINGIFY_HELPER(x) #x
# define __E_LOG_STRINGIFY(x) __E_LOG_STRINGIFY_HELPER (x)
# define __E_LOG_FILE_LINE " " __E_LOG_COLOUR_GREY "(" __FILE__ ":" __E_LOG_STRINGIFY (__LINE__) ")" __E_LOG_COLOUR_RESET
#endif /* E_LOG_CONFIG_NO_FILE_LINE */

/* TODO when clang finally supports __VA_OPT__ properly, enable it for clang */
#if __STDC_VERSION__ >= 202000L && defined (__GNUC__)

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

#else /* __STDC_VERSION__ >= 202000L && defined (__GNUC__) */

# ifdef NDEBUG
#  define e_log_debug(...)
# else /* NDEBUG */
#  define e_log_debug(...) e_log_priv_impl (__E_LOG_FILE_LINE "\n", __E_LOG_COLOUR_MAGENTA "DEBUG " __E_LOG_COLOUR_RESET __VA_ARGS__)
# endif /* NDEBUG */
# define e_log_info(...)  e_log_priv_impl (__E_LOG_FILE_LINE "\n", __E_LOG_COLOUR_GREEN   "INFO  " __E_LOG_COLOUR_RESET __VA_ARGS__)
# define e_log_warn(...)  e_log_priv_impl (__E_LOG_FILE_LINE "\n", __E_LOG_COLOUR_YELLOW  "WARN  " __E_LOG_COLOUR_RESET __VA_ARGS__)
# define e_log_error(...) e_log_priv_impl (__E_LOG_FILE_LINE "\n", __E_LOG_COLOUR_RED     "ERROR " __E_LOG_COLOUR_RESET __VA_ARGS__)
# define e_die(...)                                                            \
	do {                                                                   \
		e_log_error (__VA_ARGS__);                                     \
		exit (EXIT_FAILURE);                                           \
	} while (0)

#if defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
void e_log_priv_impl (const char *file_pos, const char *fmt, ...);
#else /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
__attribute__ ((format (printf, 2, 3)))
void e_log_priv_impl (const char *file_pos, const char *fmt, ...);
#endif /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */

#endif /* __STDC_VERSION__ >= 202000L && defined (__GNUC__) */

/* implementation *************************************************************/

#ifdef E_LOG_IMPL
# if __STDC_VERSION__ < 202000L || !defined (__GNUC__)

# include <stdarg.h>

#  if !(defined (__MINGW32__) || defined (_WIN32) || defined (WIN32))
__attribute__ ((format (printf, 2, 3)))
#  endif /* !(defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)) */
void
e_log_priv_impl (const char *file_pos, const char *fmt, ...)
{
	va_list ap;

	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, "%s", file_pos);
}

# endif /* __STDC_VERSION__ < 202000L || !defined (__GNUC__) */
#endif /* E_LOG_IMPL */

#endif /* _EMPOWER_LOG_H_ */
