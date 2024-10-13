#include "e_log.h"

#if E_CONFIG_MODULE_LOG
#if E_STDC_VERSION < E_STDC_VERSION_C23 || !defined (E_COMPILER_GCC)

#include <stdarg.h>

void
__e_log_impl (const char *file_pos, const char *fmt, ...)
{
	va_list ap;

	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, "%s", file_pos);
}

#endif /* E_STDC_VERSION < E_STDC_VERSION_C23 || !defined (E_COMPILER_GCC) */
#endif /* E_CONFIG_MODULE_LOG */
