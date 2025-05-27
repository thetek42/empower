#ifndef _EMPOWER_TEST_H_
#define _EMPOWER_TEST_H_

/*! e_test ********************************************************************
 *
 * This module provides basic functionality for unit testing using macros. Type
 * inference can be enabled when using C23 for cleaner code.
 *
 * Dependencies:
 *  - e_result (optional)
 *
 * Configuration options:
 *  - E_CONFIG_TEST_TYPE_INFERENCE: Enable type inference in C23
 *  - E_CONFIG_TEST_NO_E_RESULT: Disable E_Result support
 *
 ******************************************************************************/

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef E_CONFIG_TEST_NO_E_RESULT
# include <e_result.h>
#endif /* E_CONFIG_TEST_NO_E_RESULT */

/* public interface ***********************************************************/

struct e_test_result {
	size_t success;
	size_t failure;
};

extern struct e_test_result e_global_test;

void e_test_finish (void);
void (e_test_assert) (const char *name, const char *location, const char *expr, bool value);
void (e_test_assert_ptr_eq) (const char *name, const char *location, const char *expr, const void *got, const void *want);
void (e_test_assert_str_eq) (const char *name, const char *location, const char *expr, const char *got, const char *want);
void (e_test_assert_mem_eq) (const char *name, const char *location, const char *expr, const char *got, const char *want, size_t len);
void (e_test_assert_ptr_aligned) (const char *name, const char *location, const char *expr, const void *ptr, uintptr_t alignment);
#ifndef E_CONFIG_TEST_NO_E_RESULT
void (e_test_assert_ok) (const char *name, const char *location, const char *expr, E_Result result);
void (e_test_assert_err) (const char *name, const char *location, const char *expr, E_Result result);
#endif /* E_CONFIG_TEST_NO_E_RESULT */

void e_test_priv_print_name (const char *name, const char *location);
void e_test_priv_print_assert_fn (const char *fn_name);

#define __E_TEST_STRINGIFY_HELPER(x) #x
#define __E_TEST_STRINGIFY(x) __E_TEST_STRINGIFY_HELPER (x)
#define __E_TEST_FILE_LINE __FILE__ ":" __E_TEST_STRINGIFY (__LINE__)

#define e_test_assert(name, expr) (e_test_assert) (name, __E_TEST_FILE_LINE, #expr, expr)
#define e_test_assert_ptr_eq(name, expr, check) (e_test_assert_ptr_eq) (name, __E_TEST_FILE_LINE, #expr, expr, check)
#define e_test_assert_null(name, expr) e_test_assert_ptr_eq (name, expr, NULL)
#define e_test_assert_str_eq(name, expr, check) (e_test_assert_str_eq) (name, __E_TEST_FILE_LINE, #expr, expr, check)
#define e_test_assert_mem_eq(name, expr, check, len) (e_test_assert_mem_eq) (name, __E_TEST_FILE_LINE, #expr, expr, check, len)
#define e_test_assert_ptr_aligned(name, expr, alignment) (e_test_assert_ptr_aligned) (name, __E_TEST_FILE_LINE, #expr, expr, alignment)
#ifndef E_CONFIG_TEST_NO_E_RESULT
# define e_test_assert_ok(name, expr) (e_test_assert_ok) (name, __E_TEST_FILE_LINE, #expr, expr)
# define e_test_assert_err(name, expr) (e_test_assert_err) (name, __E_TEST_FILE_LINE, #expr, expr)
#endif /* E_CONFIG_TEST_NO_E_RESULT */

#if __STDC_VERSION__ >= 201112L
# define __E_TEST_AUTO_FMT(value) _Generic ((value),                           \
		int8_t: "%" PRIi8,                                             \
		int16_t: "%" PRIi16,                                           \
		int32_t: "%" PRIi32,                                           \
		int64_t: "%" PRIi64,                                           \
		uint8_t: "%" PRIu8,                                            \
		uint16_t: "%" PRIu16,                                          \
		uint32_t: "%" PRIu32,                                          \
		uint64_t: "%" PRIu64,                                          \
		float: "%f",                                                   \
		double: "%g",                                                  \
		long double: "%lg",                                            \
		bool: "%d",                                                    \
		default: "%p")
# define __E_TEST_ASSERT_EQ_PRINT_VALUES(expr, result, other)                  \
	do {                                                                   \
		fprintf (stderr, "%s \x1b[36mgot\x1b[0m ", #expr);             \
		fprintf (stderr, __E_TEST_AUTO_FMT (result), result);          \
		fprintf (stderr, " \x1b[36mwant\x1b[0m ");                     \
		fprintf (stderr, __E_TEST_AUTO_FMT (other), other);            \
	} while (0)
#else /* __STDC_VERSION__ >= 201112L */
# define __E_TEST_ASSERT_EQ_PRINT_VALUES(expr, a, b)                           \
	fprintf (stderr, "\x1b[36mfailed\x1b[0m")
#endif /* __STDC_VERSION__ >= 201112L */

#define __e_test_assert_eq(name, type, expr, check)                            \
	do {                                                                   \
		type result = (expr);                                          \
		type other = (check);                                          \
		if (result == other) {                                         \
			e_global_test.success += 1;                            \
		} else {                                                       \
			e_global_test.failure += 1;                            \
			e_test_priv_print_name (name, __E_TEST_FILE_LINE);     \
			e_test_priv_print_assert_fn ("assert_eq");             \
			__E_TEST_ASSERT_EQ_PRINT_VALUES (expr, result, other); \
			fprintf (stderr, "\n");                                \
		}                                                              \
	} while (0)

#define __e_test_assert_not_eq(name, type, expr, check)                        \
	do {                                                                   \
		type result = (expr);                                          \
		type other = (check);                                          \
		if (result != other) {                                         \
			e_global_test.success += 1;                            \
		} else {                                                       \
			e_global_test.failure += 1;                            \
			e_test_priv_print_name (name, __E_TEST_FILE_LINE);     \
			e_test_priv_print_assert_fn ("assert_not_eq");         \
			__E_TEST_ASSERT_EQ_PRINT_VALUES (expr, result, other); \
			fprintf (stderr, "\n");                                \
		}                                                              \
	} while (0)

#if __STDC_VERSION__ >= 202000L && E_CONFIG_TEST_TYPE_INFERENCE
# define e_test_assert_eq(name, expr, check) __e_test_assert_eq (name, auto, expr, check);
# define e_test_assert_not_eq(name, expr, check) __e_test_assert_not_eq (name, auto, expr, check);
#else /* __STDC_VERSION__ >= 202000L && E_CONFIG_TEST_TYPE_INFERENCE */
# define e_test_assert_eq(name, type, expr, check) __e_test_assert_eq (name, type, expr, check);
# define e_test_assert_not_eq(name, type, expr, check) __e_test_assert_not_eq (name, type, expr, check);
#endif /* __STDC_VERSION__ >= 202000L && E_CONFIG_TEST_TYPE_INFERENCE */

/* implementation *************************************************************/

#ifdef E_TEST_IMPL

struct e_test_result e_global_test = {0};

void
e_test_finish (void)
{
	double success, failure, total_perc, success_rate, failure_rate;

	success = (double) e_global_test.success;
	failure = (double) e_global_test.failure;
	total_perc = (success + failure) / 100.0;
	success_rate = success / total_perc;
	failure_rate = failure / total_perc;

	fprintf (stderr,
	         "\n=======================================\n"
	         "TEST SUMMARY:"
	         "\n - total:   %zu"
		 "\n - \x1b[32msuccess: %zu (%.2f%%)\x1b[0m"
		 "\n - \x1b[31mfailure: %zu (%.2f%%)\x1b[0m"
	         "\n=======================================\n",
		 e_global_test.success + e_global_test.failure,
		 e_global_test.success, success_rate,
		 e_global_test.failure, failure_rate);

	exit (e_global_test.failure > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}

void
(e_test_assert) (const char *name, const char *location, const char *expr,
                 bool value)
{
	if (value) {
		e_global_test.success += 1;
	} else {
		e_global_test.failure += 1;
		e_test_priv_print_name (name, location);
		e_test_priv_print_assert_fn ("assert");
		fprintf (stderr, "%s\n", expr);
	}
}

void
(e_test_assert_ptr_eq) (const char *name, const char *location,
                        const char *expr, const void *got, const void *want)
{
	if (got == want) {
		e_global_test.success += 1;
	} else {
		e_global_test.failure += 1;
		e_test_priv_print_name (name, location);
		e_test_priv_print_assert_fn ("assert_ptr_eq");
		fprintf (stderr, "%s \x1b[36mgot\x1b[0m %p "
		         "\x1b[36mwant\x1b[0m %p\n", expr, got, want);
	}
}

void
(e_test_assert_str_eq) (const char *name, const char *location,
                        const char *expr, const char *got, const char *want)
{
	if (got == want || (got && want && !strcmp (got, want))) {
		e_global_test.success += 1;
	} else {
		e_global_test.failure += 1;
		e_test_priv_print_name (name, location);
		e_test_priv_print_assert_fn ("assert_str_eq");
		fprintf (stderr, "%s \x1b[36mgot\x1b[0m %s "
		         "\x1b[36mwant\x1b[0m %s\n", expr, got, want);
	}
}

void
(e_test_assert_mem_eq) (const char *name, const char *location,
                        const char *expr, const char *got, const char *want,
                        size_t len)
{
	(void) expr;
	if (got == want || (got && want && !memcmp (got, want, len))) {
		e_global_test.success += 1;
	} else {
		e_global_test.failure += 1;
		e_test_priv_print_name (name, location);
		e_test_priv_print_assert_fn ("assert_mem_eq");
		fprintf (stderr, "\n");
	}
}

void
(e_test_assert_ptr_aligned) (const char *name, const char *location,
                             const char *expr, const void *ptr,
                             uintptr_t alignment)
{
	if (((uintptr_t) ptr % alignment) == 0) {
		e_global_test.success += 1;
	} else {
		e_global_test.failure += 1;
		e_test_priv_print_name (name, location);
		e_test_priv_print_assert_fn ("assert_ptr_aligned");
		fprintf (stderr, "%s = \x1b[36m%p\x1b[0m not aligned to "
		         "%zu bytes\n", expr, ptr, alignment);
	}
}

#ifndef E_CONFIG_TEST_NO_E_RESULT
void
(e_test_assert_ok) (const char *name, const char *location, const char *expr,
                    E_Result result)
{
	if (result == E_OK) {
		e_global_test.success += 1;
	} else {
		e_global_test.failure += 1;
		e_test_priv_print_name (name, location);
		e_test_priv_print_assert_fn ("assert_ok");
		fprintf (stderr, "%s \x1b[36mreturned error\x1b[0m %s\n",
		         expr, e_result_to_str (result));
	}
}

void
(e_test_assert_err) (const char *name, const char *location, const char *expr,
                     E_Result result)
{
	if (result != E_OK) {
		e_global_test.success += 1;
	} else {
		e_global_test.failure += 1;
		e_test_priv_print_name (name, location);
		e_test_priv_print_assert_fn ("assert_err");
		fprintf (stderr, "%s \x1b[36mreturned ok\x1b[0m\n", expr);
	}
}
#endif /* E_CONFIG_TEST_NO_E_RESULT */

void
e_test_priv_print_name (const char *name, const char *location)
{
	fprintf (stderr, "\x1b[31mfail\x1b[0m \x1b[33m%s\x1b[0m (%s) ",
	         name, location);
}

void
e_test_priv_print_assert_fn (const char *fn_name)
{
	fprintf (stderr, "\x1b[35m%s\x1b[0m ", fn_name);
}

#endif /* E_TEST_IMPL */

#endif /* _EMPOWER_TEST_H_ */
