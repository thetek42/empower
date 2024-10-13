#include "e_log.h"

#if E_CONFIG_MODULE_LOG
#if C_STDC_VERSION < C_STDC_VERSION_C23 || !defined (C_COMPILER_GCC)

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

#endif /* C_STDC_VERSION < C_STDC_VERSION_C23 || !defined (C_COMPILER_GCC) */
#endif /* E_CONFIG_MODULE_LOG */
