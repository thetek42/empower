#ifndef _EMPOWER_DEBUG_H_
#define _EMPOWER_DEBUG_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_DEBUG

#if E_STDC_VERSION >= E_STDC_VERSION_C11

# define E_DEBUG_AUTO_FMT(value) _Generic ((value),                            \
		i8: "%" PRIi8,                                                 \
		i16: "%" PRIi16,                                               \
		i32: "%" PRIi32,                                               \
		i64: "%" PRIi64,                                               \
		u8: "%" PRIu8,                                                 \
		u16: "%" PRIu16,                                               \
		u32: "%" PRIu32,                                               \
		u64: "%" PRIu64,                                               \
		f32: "%f",                                                     \
		f64: "%f",                                                     \
		bool: "%d",                                                    \
		default: "%p")

# define e_debug(value)                                                        \
	do {                                                                   \
		fprintf (stderr, "\x1b[35mDEBUG \x1b[0m"                       \
		         E_MACRO_STRINGIFY (value) " = \x1b[35m");             \
		fprintf (stderr, E_DEBUG_AUTO_FMT(value), (value));            \
		fprintf (stderr, "\x1b[0m \x1b[90m(" __FILE__ ":"              \
		         E_MACRO_STRINGIFY (__LINE__) ")\x1b[0m\n");           \
	} while (0)

#endif /* E_STDC_VERSION >= E_STDC_VERSION_C11 */

#endif /* E_CONFIG_MODULE_DEBUG */
#endif /* _EMPOWER_DEBUG_H_ */
