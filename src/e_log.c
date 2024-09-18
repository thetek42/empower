#include "empower.h"
#include <stdarg.h>
#include <stdio.h>

#if E_CONFIG_MODULE_LOG

#if E_STDC_VERSION < E_STDC_VERSION_C23
# if E_STDC_VERSION >= E_STDC_VERSION_C99

void
__e_log_impl (const char *formatted_file_pos, const char *fmt, ...)
{
		va_list ap;

		va_start (ap, (fmt));
		vfprintf (stderr, (fmt), ap);
		va_end (ap);
		fprintf (stderr, formatted_file_pos);
}

# else /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

#  define LOG_IMPL(level, fmt, suffix)                                         \
	do {                                                                   \
		va_list ap;                                                    \
		fprintf (stderr, level);                                       \
		va_start (ap, (fmt));                                          \
		vfprintf (stderr, (fmt), ap);                                  \
		va_end (ap);                                                   \
		fprintf (stderr, (suffix));                                    \
	} while (0)

void
e_log_debug (const char *fmt, ...)
{
	LOG_IMPL ("\x1b[35mDEBUG \x1b[0m", fmt, "\n");
}

void
e_log_info (const char *fmt, ...)
{
	LOG_IMPL ("\x1b[32mINFO  \x1b[0m", fmt, "\n");
}

void
e_log_warn (const char *fmt, ...)
{
	LOG_IMPL ("\x1b[33mWARN  \x1b[0m", fmt, "\n");
}

void
e_log_error (const char *fmt, ...)
{
	LOG_IMPL ("\x1b[31mERROR \x1b[0m", fmt, "\n");
}

void
e_die (const char *fmt, ...)
{
	LOG_IMPL ("\x1b[31mERROR \x1b[0m", fmt, "\n");
	exit (EXIT_FAILURE);
}

# endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */
#endif /* E_STDC_VERSION <= E_STDC_VERSION_C23 */

#endif /* E_CONFIG_MODULE_LOG */
