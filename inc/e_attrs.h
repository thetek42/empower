#ifndef _EMPOWER_ATTRS_H_
#define _EMPOWER_ATTRS_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

/*! e_attrs *******************************************************************
 * 
 * This module provides support for using attributes in different compilers and
 * different versions of the C standard.
 *
 ******************************************************************************/

/* c23 ************************************************************************/

#if E_STDC_VERSION >= E_STDC_VERSION_C23

# define E_ATTR_DEPRECATED(...) [[deprecated (__VA_ARGS__)]]
# define E_ATTR_FALLTHROUGH [[fallthrough]]
# define E_ATTR_MAYBE_UNUSED [[maybe_unused]]
# define E_ATTR_NODISCARD(...) [[nodiscard (__VA_ARGS__)]]
# define E_ATTR_NORETURN [[noreturn]]
# define E_ATTR_REPRODUCIBLE [[reproducible]]
# define E_ATTR_UNSEQUENCED [[unsequenced]]

# if defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG)
#  define E_ATTR_FORMAT(function, fmt_arg, va_arg) [[gnu::format (function, fmt_arg, va_arg)]]
# else /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */
#  define E_ATTR_FORMAT(function, fmt_arg, va_arg)
# endif /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */

#else /* E_STDC_VERSION < E_STDC_VERSION_C23 */

/* compiler builtins **********************************************************/

# define E_ATTR_NORETURN _Noreturn

# if defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG)
#  define E_ATTR_DEPRECATED(...) __attribute__ ((deprecated (__VA_ARGS__)))
#  define E_ATTR_FALLTHROUGH __attribute__ ((fallthrough))
#  define E_ATTR_MAYBE_UNUSED __attribute__ ((unused))
#  define E_ATTR_NODISCARD(...) __attribute__ ((warn_unused_result))
#  define E_ATTR_REPRODUCIBLE __attribute__ ((reproducible))
#  define E_ATTR_UNSEQUENCED __attribute__ ((unsequenced))
#  define E_ATTR_FORMAT(function, fmt_arg, va_arg) __attribute__ ((format (function, fmt_arg, va_arg)))
# else /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */
#  define E_ATTR_DEPRECATED(...) __declspec (deprecated (__VA_ARGS__))
#  define E_ATTR_FALLTHROUGH
#  define E_ATTR_MAYBE_UNUSED
#  define E_ATTR_NODISCARD(...)
#  define E_ATTR_REPRODUCIBLE
#  define E_ATTR_UNSEQUENCED
#  define E_ATTR_FORMAT(function, fmt_arg, va_arg)
# endif /* defined (E_COMPILER_GCC) || defined (E_COMPILER_CLANG) */

#endif /* E_STDC_VERSION < E_STDC_VERSION_C23 */

#endif /* _EMPOWER_ATTRS_H_ */
