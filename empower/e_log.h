#ifndef EMPOWER_LOG_H_
#define EMPOWER_LOG_H_

/******************************************************************************
 *
 * This module provides functions and macros for logging. There are four levels
 * for logging: debug, info, warn and error. Log messages are printed to
 * `stderr`. The same formatting syntax as `fprintf` is used. In addition, this
 * module provides the `e_die` macro, which can be used for terminating the
 * programme with an error message. Debug log messages are only shown in debug
 * mode (`NDEBUG` not defined).
 *
 * Configuration options:
 *  - `E_CONFIG_LOG_NO_COLOUR`: Disable ANSI escape code colouring.
 *  - `E_CONFIG_LOG_NO_FILE_LINE`: Disable source location in the log.
 *
 ******************************************************************************/

#if !defined (__STDC_VERSION__) || __STDC_VERSION__ < 199901L
# error e_log requires C99 or newer
#endif

#include <stdio.h>
#include <stdlib.h>

#if E_CONFIG_LOG_NO_COLOUR
# define E_LOG_COLOUR_RESET_ ""
# define E_LOG_COLOUR_RED_ ""
# define E_LOG_COLOUR_GREEN_ ""
# define E_LOG_COLOUR_YELLOW_ ""
# define E_LOG_COLOUR_MAGENTA_ ""
# define E_LOG_COLOUR_GREY_ ""
#else /* E_CONFIG_LOG_NO_COLOUR */
# define E_LOG_COLOUR_RESET_ "\x1b[0m"
# define E_LOG_COLOUR_RED_ "\x1b[31m"
# define E_LOG_COLOUR_GREEN_ "\x1b[32m"
# define E_LOG_COLOUR_YELLOW_ "\x1b[33m"
# define E_LOG_COLOUR_MAGENTA_ "\x1b[35m"
# define E_LOG_COLOUR_GREY_ "\x1b[90m"
#endif /* E_CONFIG_LOG_NO_COLOUR */

#if E_CONFIG_LOG_NO_FILE_LINE
# define E_LOG_FILE_LINE_ ""
#else /* E_CONFIG_LOG_NO_FILE_LINE */
# ifndef E_STRINGIFY
#  define E_STRINGIFY_HELPER_(x) #x
#  define E_STRINGIFY(x) E_STRINGIFY_HELPER_ (x)
# endif
# ifndef E_SOURCE_LOCATION
#  define E_SOURCE_LOCATION __FILE__ ":" E_STRINGIFY (__LINE__)
# endif
# define E_LOG_FILE_LINE_ " " E_LOG_COLOUR_GREY_ "(" E_SOURCE_LOCATION ")" E_LOG_COLOUR_RESET_
#endif /* E_CONFIG_LOG_NO_FILE_LINE */

#ifdef NDEBUG
# define e_log_debug(...)
#else /* NDEBUG */
# define e_log_debug(...) e_log_impl_ (E_LOG_FILE_LINE_ "\n", E_LOG_COLOUR_MAGENTA_ "DEBUG " E_LOG_COLOUR_RESET_ __VA_ARGS__)
#endif /* NDEBUG */
#define e_log_info(...)   e_log_impl_ (E_LOG_FILE_LINE_ "\n", E_LOG_COLOUR_GREEN_   "INFO  " E_LOG_COLOUR_RESET_ __VA_ARGS__)
#define e_log_warn(...)   e_log_impl_ (E_LOG_FILE_LINE_ "\n", E_LOG_COLOUR_YELLOW_  "WARN  " E_LOG_COLOUR_RESET_ __VA_ARGS__)
#define e_log_error(...)  e_log_impl_ (E_LOG_FILE_LINE_ "\n", E_LOG_COLOUR_RED_     "ERROR " E_LOG_COLOUR_RESET_ __VA_ARGS__)
#define e_die(...) \
	do { \
		e_log_error (__VA_ARGS__); \
		exit (EXIT_FAILURE); \
	} while (0)

#if defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)
void e_log_priv_impl (const char *location, const char *fmt, ...);
#else /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */
__attribute__ ((format (printf, 2, 3)))
void e_log_impl_ (const char *location, const char *fmt, ...);
#endif /* defined (__MINGW32__) || defined (_WIN32) || defined (WIN32) */

/* implementation *************************************************************/

#ifdef E_LOG_IMPL

#if __STDC_VERSION__ < 202000L || !defined (__GNUC__)

# include <stdarg.h>

# if !(defined (__MINGW32__) || defined (_WIN32) || defined (WIN32))
__attribute__ ((format (printf, 2, 3)))
# endif /* !(defined (__MINGW32__) || defined (_WIN32) || defined (WIN32)) */
void
e_log_impl_ (const char *location, const char *fmt, ...)
{
	va_list ap;

	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, "%s", location);
}

#endif /* __STDC_VERSION__ < 202000L || !defined (__GNUC__) */

#endif /* E_LOG_IMPL */

#endif /* EMPOWER_LOG_H_ */
