#ifndef _EMPOWER_TEST_H_
#define _EMPOWER_TEST_H_

/*! e_test ********************************************************************
 * 
 * This module provides basic functionality for unit testing using macros. Type
 * inference can be enabled when using C23 for cleaner code.
 *
 * Configuration options:
 *  - E_TEST_CONFIG_TYPE_INFERENCE: Enable type inference in C23
 *
 ******************************************************************************/

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* public interface ***********************************************************/

struct e_test_result {
	size_t success;
	size_t failure;
};

extern struct e_test_result e_global_test;

#define __E_TEST_STRINGIFY_HELPER(x) #x
#define __E_TEST_STRINGIFY(x) __E_TEST_STRINGIFY_HELPER (x)
#define __E_TEST_FILE_LINE __FILE__ ":" __E_TEST_STRINGIFY (__LINE__)
#define __E_TEST_ASSERT_FMT(name) "\x1b[31mfail\x1b[0m \x1b[33m" name "\x1b[0m (" __E_TEST_FILE_LINE ") "
#define __E_TEST_SPACE "                                                                      "
#define __E_TEST_PRINT_ASSERT_FN(name) fprintf (stderr, "\x1b[35m%s\x1b[0m ", name)

/**
 * Assert that a condition is truthy and update the global test statistics. If
 * the check fails, an error message will be printed.
 */
#define e_test_assert(name, expr)                                              \
	do {                                                                   \
		if ((expr)) {                                                  \
			e_global_test.success += 1;                            \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l, __E_TEST_SPACE);           \
			__E_TEST_PRINT_ASSERT_FN ("assert");                   \
			fprintf (stderr, #expr "\n");                          \
			e_global_test.failure += 1;                            \
		}                                                              \
	} while (0)

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

/**
 * Assert that two values equal each other and update the global test
 * statistics. The types of the values are determined automatically. If the
 * check fails, an error message will be printed.
 */
#define __e_test_assert_eq(name, type, expr, check)                            \
	do {                                                                   \
		type result = (expr);                                          \
		type other = (check);                                          \
		if (result == other) {                                         \
			e_global_test.success += 1;                            \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_eq");                \
			__E_TEST_ASSERT_EQ_PRINT_VALUES (expr, result, other); \
			fprintf (stderr, "\n");                                \
			e_global_test.failure += 1;                            \
		}                                                              \
	} while (0)

/**
 * Assert that two values do not equal each other and update the global test
 * statistics. The types of the values are determined automatically. If the
 * check fails, an error message will be printed.
 */
#define __e_test_assert_not_eq(name, type, expr, check)                        \
	do {                                                                   \
		type result = (expr);                                          \
		type other = (check);                                          \
		if (result != other) {                                         \
			e_global_test.success += 1;                            \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_eq");                \
			__E_TEST_ASSERT_EQ_PRINT_VALUES (expr, result, other); \
			fprintf (stderr, "\n");                                \
			e_global_test.failure += 1;                            \
		}                                                              \
	} while (0)

/**
 * Assert that two pointers equal each other and update the global test
 * statistics. If the check fails, an error message will be printed.
 */
#define e_test_assert_ptr_eq(name, expr, check)                                \
	do {                                                                   \
		const void *result = (expr);                                   \
		const void *other = (check);                                   \
		if (result == other) {                                         \
			e_global_test.success += 1;                            \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_ptr_eq");            \
			fprintf (stderr, "%s \x1b[36mgot\x1b[0m %p "           \
			         "\x1b[36mwant\x1b[0m %p\n", #expr, result,    \
			         other);                                       \
			e_global_test.failure += 1;                            \
		}                                                              \
	} while (0)

/**
 * Assert that two strings equal each other and update the global test
 * statistics. If the check fails, an error message will be printed.
 */
#define e_test_assert_str_eq(name, expr, check)                                \
	do {                                                                   \
		const char *result = (expr);                                   \
		const char *other = (check);                                   \
		if (e_cstr_eq (result, other)) {                               \
			e_global_test.success += 1;                            \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_str_eq");            \
			fprintf (stderr, "%s \x1b[36mgot\x1b[0m \"%s\" "       \
			         "\x1b[36mwant\x1b[0m \"%s\"\n",               \
			         __E_TEST_STRINGIFY (expr), result, other);    \
			e_global_test.failure += 1;                            \
		}                                                              \
	} while (0)                                                                                

/**
 * Assert that an `E_Result` is `E_OK` and update the global test statistics. If
 * the check fails, an error message will be printed.
 */
#define e_test_assert_ok(name, expr)                                           \
	do {                                                                   \
		E_Result result = (expr);                                      \
		if (result == E_OK) {                                          \
			e_global_test.success += 1;                            \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_ok");                \
			fprintf (stderr, "%s \x1b[36mreturned error\x1b[0m "   \
			         "%s\n", #expr, e_result_to_str (result));     \
			e_global_test.failure += 1;                            \
		}                                                              \
	} while (0)

/**
 * Assert that an `E_Result` is not `E_OK` and update the global test
 * statistics. If the check fails, an error message will be printed.
 */
#define e_test_assert_err(name, expr)                                          \
	do {                                                                   \
		E_Result result = (expr);                                      \
		if (result != E_OK) {                                          \
			e_global_test.success += 1;                            \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_ok");                \
			fprintf (stderr, "%s \x1b[36mreturned ok\x1b[0m\n",    \
			         #expr);                                       \
			e_global_test.failure += 1;                            \
		}                                                              \
	} while (0)

/**
 * Assert that a pointer is aligned to a certain alignment and update the global
 * test statistics. If the check fails, an error message will be printed.
 */
#define e_test_assert_ptr_aligned(name, expr, alignment)                       \
	do {                                                                   \
		const void *result = (expr);                                   \
		if (e_mem_is_aligned_to (result, alignment)) {                 \
			e_global_test.success += 1;                            \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_ptr_aligned");       \
			fprintf (stderr, "%s = \x1b[36m%p\x1b[0m not aligned " \
			         "to %zu bytes\n", #expr, result,              \
			         (size_t) alignment);                          \
			e_global_test.failure += 1;                            \
		}                                                              \
	} while (0)

/**
 * Finish the tests, print the test statistics, and terminate the programme. The
 * exit code is 0 if all tests succeeded, or non-zero if there were failures.
 */
#define e_test_finish()                                                        \
	do {                                                                   \
		double total_perc = (double) (e_global_test.success +          \
		                        e_global_test.failure) / 100.0;        \
		double succ_rate = (double) e_global_test.success / total_perc;\
		double fail_rate = (double) e_global_test.failure / total_perc;\
		fprintf (stderr, "\n=======================================\n" \
		         "TEST SUMMARY:"                                       \
		         "\n - total:   %zu"                                   \
			 "\n - \x1b[32msuccess: %zu (%.2f%%)\x1b[0m"           \
			 "\n - \x1b[31mfailure: %zu (%.2f%%)\x1b[0m"           \
		         "\n=======================================\n",        \
			 e_global_test.success + e_global_test.failure,        \
			 e_global_test.success, succ_rate,                     \
			 e_global_test.failure, fail_rate);                    \
		exit (e_global_test.failure > 0 ? EXIT_FAILURE : EXIT_SUCCESS);\
	} while (0)

/**
 * Assert that a pointer is `nullptr` and update the global test statistics. If
 * the check fails, an error message will be printed.
 */
#define e_test_assert_null(name, expr) e_test_assert_ptr_eq (name, expr, NULL)

#if __STDC_VERSION__ >= 202000L && E_TEST_CONFIG_TYPE_INFERENCE
# define e_test_assert_eq(name, expr, check) __e_test_assert_eq (name, auto, expr, check);
# define e_test_assert_not_eq(name, expr, check) __e_test_assert_not_eq (name, auto, expr, check);
#else /* __STDC_VERSION__ >= 202000L && E_TEST_CONFIG_TYPE_INFERENCE */
# define e_test_assert_eq(name, type, expr, check) __e_test_assert_eq (name, type, expr, check);
# define e_test_assert_not_eq(name, type, expr, check) __e_test_assert_not_eq (name, type, expr, check);
#endif /* __STDC_VERSION__ >= 202000L && E_TEST_CONFIG_TYPE_INFERENCE */

/* implementation *************************************************************/

#ifdef E_TEST_IMPL

struct e_test_result e_global_test = {0};

#endif /* E_TEST_IMPL */

#endif /* _EMPOWER_TEST_H_ */
