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
 * `stderr`. The same formatting syntax as `fprintf` is used. The log output is
 * coloured using ANSI escape codes. The implementation and semantics slightly
 * differ depending on which version of C is being used, and thus the available
 * feature set:
 *
 * - In C23, the log messages are directly concatenated into the correct format
 *   at compile time. They are printed with a single call to `fprintf`. File and
 *   line are added at the end of the log message.
 * - In C99 and C11, there is a separate function that deals with printing the
 *   log messages. Due to the lack of __VA_OPT__, it requires two separate calls
 *   to `fprintf`. File and line are added at the end of the log message.
 * - In C89, there are four separate functions for logging, one for each log
 *   level, where each function requires three separate calls to `fprintf`. This
 *   is due to the lack of variadic macros. Unlike with the other variants, file
 *   and line are not added.
 *
 * In addition, this module provides `e_die`, with similar differences between C
 * versions as the `e_log_*` functions/macros as described above.
 *
 ******************************************************************************/

#if E_STDC_VERSION >= E_STDC_VERSION_C23

/** docgen: collect
 * Logging macros.
 *
 * @param fmt: The format string as used by `fprintf`
 * @param ...: The format arguments
 * @see `fprintf`
 */
# define e_log_debug(fmt, ...) fprintf (stderr, "\x1b[35mDEBUG \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_info(fmt, ...)  fprintf (stderr, "\x1b[32mINFO  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_warn(fmt, ...)  fprintf (stderr, "\x1b[33mWARN  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_error(fmt, ...) fprintf (stderr, "\x1b[31mERROR \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
/* docgen: collect-end */

/**
 * Log an error message and terminate the programme.
 *
 * @param fmt: The format string as used by `fprintf`
 * @param ...: The format arguments
 * @return #noreturn
 * @see `fprintf`
 */
# define e_die(fmt, ...)                                                       \
	do {                                                                   \
		e_log_error (fmt __VA_OPT__ (,) __VA_ARGS__);                  \
		exit (EXIT_FAILURE);                                           \
	} while (0)

#elif E_STDC_VERSION >= E_STDC_VERSION_C99 /* E_STDC_VERSION >= E_STDC_VERSION_C23 */

/** docgen: collect
 * Logging macros.
 *
 * @param fmt: The format string as used by `fprintf`
 * @param ...: The format arguments
 * @see `fprintf`
 */
# define e_log_debug(...) __e_log_impl (" \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", "\x1b[35mDEBUG \x1b[0m" __VA_ARGS__)
# define e_log_info(...)  __e_log_impl (" \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", "\x1b[32mINFO  \x1b[0m" __VA_ARGS__)
# define e_log_warn(...)  __e_log_impl (" \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", "\x1b[33mWARN  \x1b[0m" __VA_ARGS__)
# define e_log_error(...) __e_log_impl (" \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", "\x1b[31mERROR \x1b[0m" __VA_ARGS__)
/* docgen: collect-end */

/**
 * Log an error message and terminate the programme.
 *
 * @param fmt: The format string as used by `fprintf`
 * @param ...: The format arguments
 * @return #noreturn
 * @see `fprintf`
 */
# define e_die(...)                                                            \
	do {                                                                   \
		e_log_error (__VA_ARGS__);                                     \
		exit (EXIT_FAILURE);                                           \
	} while (0)

E_ATTR_FORMAT (printf, 2, 3)
void __e_log_impl (const char *formatted_file_pos, const char *fmt, ...);

#else /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

/** docgen: collect
 * Logging macros.
 *
 * @param fmt: The format string as used by `fprintf`
 * @param ...: The format arguments
 * @see `fprintf`
 */
E_ATTR_FORMAT (printf, 1, 2) void e_log_debug (const char *fmt, ...);
E_ATTR_FORMAT (printf, 1, 2) void e_log_info (const char *fmt, ...);
E_ATTR_FORMAT (printf, 1, 2) void e_log_warn (const char *fmt, ...);
E_ATTR_FORMAT (printf, 1, 2) void e_log_error (const char *fmt, ...);
/* docgen: collect-end */

/**
 * Log an error message and terminate the programme.
 *
 * @param fmt: The format string as used by `fprintf`
 * @param ...: The format arguments
 * @return #noreturn
 * @see `fprintf`
 */
E_ATTR_FORMAT (printf, 1, 2) 
noreturn void e_die (const char *fmt, ...);

#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

#endif /* E_CONFIG_MODULE_LOG */
#endif /* _EMPOWER_LOG_H_ */
