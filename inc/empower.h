#ifndef _EMPOWER_H_
#define _EMPOWER_H_

#include "empower_config.h"

/* c version checks ***********************************************************/

#if !defined (__STDC_VERSION__) || __STDC_VERSION__ < 202000L
# error "Empower requires C23."
#endif /* !defined (__STDC_VERSION__) || __STDC_VERSION__ < 202000L */

/* includes *******************************************************************/

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbit.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* modules ********************************************************************/

#include "e_macro.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_cstr.h"
#include "e_debug.h"
#include "e_log.h"
#include "e_result.h"
#include "e_test.h"
#include "e_vec.h"
#include "e_str.h"
#include "e_fs.h"

#endif /* _EMPOWER_H_ */
