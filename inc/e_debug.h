#ifndef _EMPOWER_DEBUG_H_
#define _EMPOWER_DEBUG_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_DEBUG

/*! e_debug *******************************************************************
 * 
 * This module provides utilities for quick debugging.
 *
 ******************************************************************************/

#if E_STDC_VERSION >= E_STDC_VERSION_C11

/**
 * Obtain the formatting argument for `printf`-like functions for the generic
 * argument \value. Works for regular integers, floats and pointers.
 *
 * @param value: The generic argument
 */
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
		f64: "%g",                                                     \
		f128: "%lg",                                                   \
		bool: "%d",                                                    \
		default: "%p")

/**
 * Debug-print the generic argument \value. For a call like `e_debug (x + y)`,
 * it will print `x + y = 42` with the regular debug format from e_log, where 42
 * is the result of the addition. Works with every type that is accepted by
 * `E_DEBUG_AUTO_FMT`.
 *
 * @param value: The generic argument
 */
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
