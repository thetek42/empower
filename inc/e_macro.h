#ifndef _EMPOWER_MACRO_H_
#define _EMPOWER_MACRO_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

#if E_STDC_VERSION >= E_STDC_VERSION_C99
# define E_FMT_USIZE "zu"
# define E_FMT_ISIZE "zi"
#else /* E_STDC_VERSION >= E_STDC_VERSION_C99 */
# define E_FMT_USIZE "lu"
# define E_FMT_ISIZE "li"
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C99 */

# define E_MACRO_STRINGIFY(x) E_MACRO_STRINGIFY_HELPER (x)
# define E_MACRO_STRINGIFY_HELPER(x) #x

#endif /* _EMPOWER_MACRO_H_ */
