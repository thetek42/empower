#include "empower.h"

#if E_STDC_VERSION < E_STDC_VERSION_C99

int
vsnprintf (char *restrict str, usize size, const char *restrict format,
           va_list ap)
{
	/* TODO: actual bound checking, likely needs custom implementation of
	 * vsnprintf */

	(void) size;

	return vsprintf (str, format, ap);
}

#endif /* E_STDC_VERSION < E_STDC_VERSION_C99 */
