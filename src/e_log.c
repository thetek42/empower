#include "empower.h"

#if E_CONFIG_MODULE_LOG

void
__e_log_impl (const char *file_pos, const char *fmt, ...)
{
	va_list ap;

	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, "%s", file_pos);
}

#endif /* E_CONFIG_MODULE_LOG */
