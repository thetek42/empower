#ifndef _EMPOWER_H_
#define _EMPOWER_H_

#include "empower_config.h"

/* c version checks ***********************************************************/

#define E_STDC_VERSION_C23 202000L /* to be replaced with 202311L in the future */
#define E_STDC_VERSION_C11 201112L
#define E_STDC_VERSION_C99 199901L
#define E_STDC_VERSION_C89 198900L

#ifdef __STDC_VERSION__
# define E_STDC_VERSION __STDC_VERSION__
#else /* __STDC_VERSION__ */
# define E_STDC_VERSION E_STDC_VERSION_C89
#endif /* __STDC_VERSION__ */

/* config checks **************************************************************/

#if E_CONFIG_TEST_TYPE_MACROS && (E_STDC_VERSION < E_STDC_VERSION_C23)
# error "E_CONFIG_TEST_TYPE_MACROS requires C23 or later"
#endif /* E_CONFIG_TEST_TYPE_MACROS && (E_STDC_VERSION < E_STDC_VERSION_C23) */

/* includes *******************************************************************/

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if E_STDC_VERSION >= E_STDC_VERSION_C99
# include <inttypes.h>
# if E_STDC_VERSION < E_STDC_VERSION_C23
#  include <stdbool.h>
# endif /* E_STDC_VERSION < E_STDC_VERSION_C23 */
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

#if E_CONFIG_HAS_SYS_TYPES
# include <sys/types.h>
#endif

/* modules ********************************************************************/

#include "e_backport.h"
#include "e_macro.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_cstr.h"
#include "e_debug.h"
#include "e_log.h"
#include "e_result.h"
#include "e_test.h"

#endif /* _EMPOWER_H_ */
