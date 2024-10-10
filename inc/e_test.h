#ifndef _EMPOWER_TEST_H_
#define _EMPOWER_TEST_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_TEST

/*! e_test ********************************************************************
 * 
 * This module provides basic functionality for unit testing using macros. Type
 * inference can be used in C23 for cleaner code.
 *
 * Module dependencies:
 *  - e_debug (optional)
 *  - e_mem (optional)   
 *  - e_result (optional)
 *
 * Configuration options:
 *  - TYPE_INFERENCE: Enable type inference in C23
 *
 ******************************************************************************/

struct __e_test_result {
	usize success;
	usize failure;
};

extern struct __e_test_result __e_global_test;

/**
 * Specify this in the main file of your test programme.
 */
#define E_TEST_MAIN() struct __e_test_result __e_global_test = {0}

#define __E_TEST_ASSERT_FMT(name) "\x1b[31mfail\x1b[0m \x1b[33m" name "\x1b[0m (" E_MACRO_FILE_LINE ") "
#define __E_TEST_SPACE "                                                                      "
#define __E_TEST_PRINT_ASSERT_FN(name) fprintf (stderr, "\x1b[35m%s\x1b[0m ", name)

/**
 * Assert that a condition is truthy and update the global test statistics. If
 * the check fails, an error message will be printed.
 */
#define e_test_assert(name, expr)                                              \
	do {                                                                   \
		if ((expr)) {                                                  \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l, __E_TEST_SPACE);           \
			__E_TEST_PRINT_ASSERT_FN ("assert");                   \
			fprintf (stderr, #expr "\n");                          \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)

#if E_STDC_VERSION >= E_STDC_VERSION_C11 && E_CONFIG_MODULE_DEBUG
# define __E_TEST_ASSERT_EQ_PRINT_VALUES(expr, result, other)                  \
	do {                                                                   \
		fprintf (stderr, "%s \x1b[36mgot\x1b[0m ", #expr);             \
		fprintf (stderr, E_DEBUG_AUTO_FMT (result), result);           \
		fprintf (stderr, " \x1b[36mwant\x1b[0m ");                     \
		fprintf (stderr, E_DEBUG_AUTO_FMT (other), other);             \
	} while (0)
#else /* E_STDC_VERSION >= E_STDC_VERSION_C11 && E_CONFIG_MODULE_DEBUG */
# define __E_TEST_ASSERT_EQ_PRINT_VALUES(expr, a, b)                           \
	fprintf (stderr, "\x1b[36mfailed\x1b[0m")
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C11 && E_CONFIG_MODULE_DEBUG */

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
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_eq");                \
			__E_TEST_ASSERT_EQ_PRINT_VALUES (expr, result, other); \
			fprintf (stderr, "\n");                                \
			__e_global_test.failure += 1;                          \
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
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_eq");                \
			__E_TEST_ASSERT_EQ_PRINT_VALUES (expr, result, other); \
			fprintf (stderr, "\n");                                \
			__e_global_test.failure += 1;                          \
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
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_ptr_eq");            \
			fprintf (stderr, "%s \x1b[36mgot\x1b[0m %p "           \
			         "\x1b[36mwant\x1b[0m %p\n", #expr, result,    \
			         other);                                       \
			__e_global_test.failure += 1;                          \
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
		if (strcmp (result, other) == 0) {                             \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_str_eq");            \
			fprintf (stderr, "%s \x1b[36mgot\x1b[0m \"%s\" "       \
			         "\x1b[36mwant\x1b[0m \"%s\"\n",               \
			         E_MACRO_STRINGIFY (expr), result, other);     \
			__e_global_test.failure += 1;                          \
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
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_ok");                \
			fprintf (stderr, "%s \x1b[36mreturned error\x1b[0m "   \
			         "%s\n", #expr, e_result_to_str (result));     \
			__e_global_test.failure += 1;                          \
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
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_ok");                \
			fprintf (stderr, "%s \x1b[36mreturned ok\x1b[0m\n",    \
			         #expr);                                       \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)

/**
 * Assert that a pointer is aligned to a certain alignment and update the global
 * test statistics. If the check fails, an error message will be printed.
 */
#define e_test_assert_ptr_aligned(name, expr, alignment)                       \
	do {                                                                   \
		const void *result = (expr);                                   \
		if (e_mem_is_aligned (result, alignment)) {                    \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name));  \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_ptr_aligned");       \
			fprintf (stderr, "%s = \x1b[36m%p\x1b[0m not aligned " \
			         "to %zu bytes\n", #expr, result,              \
			         (usize) alignment);                           \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)

/**
 * Finish the tests, print the test statistics, and terminate the programme. The
 * exit code is 0 if all tests succeeded, or non-zero if there were failures.
 */
#define e_test_finish()                                                        \
	do {                                                                   \
		f64 total_perc = (f64) (__e_global_test.success +              \
		                        __e_global_test.failure) / 100.0;      \
		f64 succ_rate = (f64) __e_global_test.success / total_perc;    \
		f64 fail_rate = (f64) __e_global_test.failure / total_perc;    \
		fprintf (stderr, "\n=======================================\n" \
		         "TEST SUMMARY:"                                       \
		         "\n - total:   %zu"                                   \
			 "\n - \x1b[32msuccess: %zu (%.2f%%)\x1b[0m"           \
			 "\n - \x1b[31mfailure: %zu (%.2f%%)\x1b[0m"           \
		         "\n=======================================\n",        \
			 __e_global_test.success + __e_global_test.failure,    \
			 __e_global_test.success, succ_rate,                   \
			 __e_global_test.failure, fail_rate);                  \
		exit (__e_global_test.failure > 0 ? EXIT_FAILURE :             \
		      EXIT_SUCCESS);                                           \
	} while (0)

/**
 * Assert that a pointer is `nullptr` and update the global test statistics. If
 * the check fails, an error message will be printed.
 */
#define e_test_assert_null(name, expr)                                         \
	e_test_assert_ptr_eq (name, expr, nullptr)

#if E_STDC_VERSION >= E_STDC_VERSION_C23 && E_CONFIG_TEST_TYPE_INFERENCE && !defined (__E_CONFIG_TEST_FORCE_TYPES)
# define e_test_assert_eq(name, expr, check) __e_test_assert_eq (name, auto, expr, check);
# define e_test_assert_not_eq(name, expr, check) __e_test_assert_not_eq (name, auto, expr, check);
#else /* E_STDC_VERSION >= E_STDC_VERSION_C23 && E_CONFIG_TEST_TYPE_INFERENCE && !defined (__E_CONFIG_TEST_FORCE_TYPES) */
# define e_test_assert_eq(name, type, expr, check) __e_test_assert_eq (name, type, expr, check);
# define e_test_assert_not_eq(name, type, expr, check) __e_test_assert_not_eq (name, type, expr, check);
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C23 && E_CONFIG_TEST_TYPE_INFERENCE && !defined (__E_CONFIG_TEST_FORCE_TYPES) */

#endif /* E_CONFIG_MODULE_TEST */
#endif /* _EMPOWER_TEST_H_ */
