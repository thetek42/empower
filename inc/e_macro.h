#ifndef _EMPOWER_MACRO_H_
#define _EMPOWER_MACRO_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

/*! e_log *********************************************************************
 * 
 * This module provides macro utilities.
 *
 ******************************************************************************/

/** docgen: collect
 * Helper macros for formatting of certain types.
 */
#if E_STDC_VERSION >= E_STDC_VERSION_C99
# define E_FMT_USIZE "zu"
# define E_FMT_ISIZE "zi"
#else /* E_STDC_VERSION >= E_STDC_VERSION_C99 */
# define E_FMT_USIZE "lu"
# define E_FMT_ISIZE "li"
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */
/* docgen: collect-end */

/**
 * Turn any expression into a string.
 *
 * @param x: The expression to stringify
 */
#define E_MACRO_STRINGIFY(x) __E_MACRO_STRINGIFY_HELPER (x)
#define __E_MACRO_STRINGIFY_HELPER(x) #x

/**
 * Concatenate any two expressions
 *
 * @param a: The first part
 * @param b: The second part
 */
#define E_MACRO_CONCAT(a, b) __E_MACRO_CONCAT_HELPER (a, b)
#define __E_MACRO_CONCAT_HELPER(a, b) a##b

#endif /* _EMPOWER_MACRO_H_ */
