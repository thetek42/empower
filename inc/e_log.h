#ifndef _EMPOWER_LOG_H_
#define _EMPOWER_LOG_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_LOG

#if E_STDC_VERSION >= E_STDC_VERSION_C23

# define e_log_debug(fmt, ...) fprintf (stderr, "\x1b[35mDEBUG \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_info(fmt, ...)  fprintf (stderr, "\x1b[32mINFO  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_warn(fmt, ...)  fprintf (stderr, "\x1b[33mWARN  \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_log_error(fmt, ...) fprintf (stderr, "\x1b[31mERROR \x1b[0m" fmt " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n" __VA_OPT__ (,) __VA_ARGS__)
# define e_die(fmt, ...)                                                       \
	do {                                                                   \
		e_log_error (fmt __VA_OPT__ (,) __VA_ARGS__);                  \
		exit (EXIT_FAILURE);                                           \
	} while (0)

#else /* E_STDC_VERSION >= E_STDC_VERSION_C23 */

enum __e_log_level {
	__E_LOG_LEVEL_DEBUG,
	__E_LOG_LEVEL_INFO,
	__E_LOG_LEVEL_WARN,
	__E_LOG_LEVEL_ERROR
};

# if E_STDC_VERSION >= E_STDC_VERSION_C99

#  define e_log_debug(...) __e_log_impl (__E_LOG_LEVEL_DEBUG, " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
#  define e_log_info(...)  __e_log_impl (__E_LOG_LEVEL_INFO,  " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
#  define e_log_warn(...)  __e_log_impl (__E_LOG_LEVEL_WARN,  " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
#  define e_log_error(...) __e_log_impl (__E_LOG_LEVEL_ERROR, " \x1b[90m(" __FILE__ ":" E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n", __VA_ARGS__)
#  define e_die(...)                                                           \
	do {                                                                   \
		e_log_error (__VA_ARGS__);                                     \
		exit (EXIT_FAILURE);                                           \
	} while (0)

void __e_log_impl (enum __e_log_level level, const char *formatted_file_pos, const char *fmt, ...);

# else /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

void e_log_debug (const char *fmt, ...);
void e_log_info (const char *fmt, ...);
void e_log_warn (const char *fmt, ...);
void e_log_error (const char *fmt, ...);
void e_die (const char *fmt, ...);

# endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

#endif /* E_STDC_VERSION >= E_STDC_VERSION_C23 */

#endif /* E_CONFIG_MODULE_LOG */
#endif /* _EMPOWER_LOG_H_ */
