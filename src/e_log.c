#include "empower.h"
#include <stdarg.h>
#include <stdio.h>

#ifndef E_STDC23

static const char *const level_str[] = {
	"\x1b[35mDEBUG \x1b[0m",
	"\x1b[32mINFO  \x1b[0m",
	"\x1b[33mWARN  \x1b[0m",
	"\x1b[31mERROR \x1b[0m",
};

void
__e_log_impl (enum __e_log_level level, const char *formatted_file_pos,
              const char *fmt, ...)
{
	va_list ap;

	fprintf (stderr, level_str[level]);
	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, formatted_file_pos);
}

#endif
