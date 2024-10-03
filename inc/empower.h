#ifndef _EMPOWER_H_
#define _EMPOWER_H_

#include "empower_config.h"

/* c version checks ***********************************************************/

#define E_STDC_VERSION_C99 199901L
#define E_STDC_VERSION_C11 201112L
#define E_STDC_VERSION_C17 201710L
#define E_STDC_VERSION_C23 202000L /* should be 202311L in the future */

#if !defined (__STDC_VERSION__) || __STDC_VERSION__ < E_STDC_VERSION_C99
# error "Empower requires at least C99 to run."
#endif /* !defined (__STDC_VERSION__) || __STDC_VERSION__ < E_STDC_VERSION_C99 */

#define E_STDC_VERSION __STDC_VERSION__

/* compiler checks ************************************************************/

#if defined (__clang__)
# define E_COMPILER_CLANG
#elif defined (__GNUC__) /* defined (__clang__) */
# define E_COMPILER_GCC
#elif defined (_MSC_VER) /* defined (__GNUC__) */
# define E_COMPILER_MSVC
#endif /*defined (_MSC_VER) */

/* includes *******************************************************************/

#define _XOPEN_SOURCE 600
#define _POSIX_C_SOURCE 200809L
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
#include <time.h>
#include <unistd.h>

/* modules ********************************************************************/

#include "e_compat.h"
#include "e_attrs.h"
#include "e_macro.h"
#include "e_types.h"
#include "e_alloc.h"
#include "e_cstr.h"
#include "e_debug.h"
#include "e_enc.h"
#include "e_log.h"
#include "e_math.h"
#include "e_result.h"
#include "e_test.h"
#include "e_vec.h"
#include "e_arena.h"
#include "e_fs.h"
#include "e_str.h"
#include "e_ini.h"

#endif /* _EMPOWER_H_ */
