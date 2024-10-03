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

# define nullptr ((nullptr_t) NULL)
typedef void *nullptr_t; /* not exactly the same behaviour but whatever */

# if E_STDC_VERSION >= E_STDC_VERSION_C11
#  include <stdalign.h>
# endif /* E_STDC_VERSION >= E_STDC_VERSION_C11 */

#endif /* E_STDC_VERSION < E_STDC_VERSION_C23 */

/* c11 backports **************************************************************/

#if E_STDC_VERSION < E_STDC_VERSION_C11

# if defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG)
#  define static_assert(expr, msg) typedef char E_MACRO_CONCAT (__e_static_assert_, __COUNTER__)[(expr) ? 1 : -1]
# else /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */
#  define static_assert(expr, msg) typedef char E_MACRO_CONCAT (__e_static_assert_, __LINE__)[(expr) ? 1 : -1]
# endif /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */

# if defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG)
#  define alignof(expr) __alignof__ (expr)
# elif defined (E_COMPILER_MSVC) /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */
#  define alignof(expr) __alignof (expr)
# else /* defined (E_COMPILER_MSVC) */
#  define __E_ALIGNOF_NOT_SUPPORTED
# endif /* defined (E_COMPILER_MSVC) */

#endif /* E_STDC_VERSION < E_STDC_VERSION_C11 */

/* miscellaneous **************************************************************/

#if E_STDC_VERSION >= E_STDC_VERSION_C11
# define E_MAX_ALIGN alignof (max_align_t)
#elif defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) /* E_STDC_VERSION >= E_STDC_VERSION_C11 */
# define E_MAX_ALIGN __BIGGEST_ALIGNMENT__
#else /* defined (E_COMPILER_GCC) || define (E_COMPILER_CLANG) */
# define E_MAX_ALIGN sizeof (uint64_t)
#endif /* defined (E_COMPILER_GCC) || define (E_COMPILER_CLANG) */

#endif /* _EMPOWER_COMPAT_H_ */
