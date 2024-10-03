#ifndef _EMPOWER_COMPAT_H_
#define _EMPOWER_COMPAT_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

/*! e_compat ******************************************************************
 * 
 * This module provides compatibility for different versions of the C standard,
 * mostly by backporting features or using compiler-specific implemenations.
 *
 ******************************************************************************/

/* c23 backports **************************************************************/

#if E_STDC_VERSION < E_STDC_VERSION_C23

#define nullptr ((nullptr_t) NULL)
typedef void *nullptr_t; /* not exactly the same behaviour but whatever */

#endif /* E_STDC_VERSION < E_STDC_VERSION_C23 */

#endif /* _EMPOWER_COMPAT_H_ */
