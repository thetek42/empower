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
 * coloured using ANSI escape codes. File and line are added at the end of each
 * log message.
 *
 * In addition, this module provides `e_die`, with similar differences between C
 * versions as the `e_log_*` functions/macros as described above.
 *
 ******************************************************************************/

/** docgen: collect
 * Logging macros.
 *
 * @param fmt: The format string as used by `fprintf`
 * @param ...: The format arguments
 * @see `fprintf`
 */
#define e_log_debug(fmt, ...) fprintf (stderr, "\x1b[35mDEBUG \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
#define e_log_info(fmt, ...)  fprintf (stderr, "\x1b[32mINFO  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
#define e_log_warn(fmt, ...)  fprintf (stderr, "\x1b[33mWARN  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
#define e_log_error(fmt, ...) fprintf (stderr, "\x1b[31mERROR \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
/* docgen: collect-end */

/**
 * Log an error message and terminate the programme.
 *
 * @param fmt: The format string as used by `fprintf`
 * @param ...: The format arguments
 * @return #noreturn
 * @see `fprintf`
 */
#define e_die(fmt, ...)                                                        \
	do {                                                                   \
		e_log_error (fmt __VA_OPT__ (,) __VA_ARGS__);                  \
		exit (EXIT_FAILURE);                                           \
	} while (0)

#endif /* E_CONFIG_MODULE_LOG */
#endif /* _EMPOWER_LOG_H_ */
