#ifndef _EMPOWER_TEST_H_
#define _EMPOWER_TEST_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_TEST

/*! e_test ********************************************************************
 * 
 * This module provides basic functionality for unit testing using macros. When
 * C23 is used, types inference is used for cleaner code. This behaviour can be
 * deactivated in the config or by adding the following line before the include
 * statement of `empower.h`:
 * | #define E_CONFIG_TEST_TYPE_MACROS_OVERRIDE
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

#define __E_TEST_ASSERT_FMT(name) "\x1b[31mfail\x1b[0m \x1b[33m" name "\x1b[0m (" __FILE__ ":%d) "
#define __E_TEST_SPACE "                                                                      "
#define __E_TEST_PRINT_ASSERT_FN(name) fprintf (stderr, "\x1b[35m%s\x1b[0m ", name)

#define __e_test_assert(type, name, expr)                                      \
	do {                                                                   \
		type success = (expr);                                         \
		if (success) {                                                 \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name),   \
			                 __LINE__);                            \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l, __E_TEST_SPACE);           \
			__E_TEST_PRINT_ASSERT_FN ("assert");                   \
			fprintf (stderr, #expr "\n");                          \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)

#define __e_test_assert_eq(type, type_cast, name, expr, check)                 \
	do {                                                                   \
		type result = (expr);                                          \
		type other = (type_cast) (check);                              \
		if (result == other) {                                         \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name),   \
			                 __LINE__);                            \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_eq");                \
			fprintf (stderr, #expr " \x1b[36mgot\x1b[0m ");        \
			fprintf (stderr, E_DEBUG_AUTO_FMT (result), result);   \
			fprintf (stderr, " \x1b[36mwant\x1b[0m ");             \
			fprintf (stderr, E_DEBUG_AUTO_FMT (other), other);     \
			fprintf (stderr, "\n");                                \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)

#define e_test_assert_str_eq(name, expr, check)                                \
	do {                                                                   \
		const char *result = (expr);                                   \
		const char *other = (check);                                   \
		if (strcmp (result, other) == 0) {                             \
			__e_global_test.success += 1;                          \
		} else {                                                       \
			int p = fprintf (stderr, __E_TEST_ASSERT_FMT (name),   \
			                 __LINE__);                            \
			int l = (int) strlen (__E_TEST_SPACE) - p;             \
			fprintf (stderr, "%.*s", l > 0 ? l : 0,                \
			         __E_TEST_SPACE);                              \
			__E_TEST_PRINT_ASSERT_FN ("assert_str_eq");            \
			fprintf (stderr, #expr " \x1b[36mgot\x1b[0m \"%s\" "   \
			         "\x1b[36mwant\x1b[0m \"%s\"\n", result,       \
			         other);                                       \
			__e_global_test.failure += 1;                          \
		}                                                              \
	} while (0)                                                                                

/**
 * Finish the tests by printing out a summary of how many tests were successful
 * and how many tests failed.
 */
#define e_test_finish()                                                        \
	do {                                                                   \
		f64 total_perc = (f64) (__e_global_test.success +              \
		                        __e_global_test.failure) / 100.0;      \
		f64 succ_rate = (f64) __e_global_test.success / total_perc;    \
		f64 fail_rate = (f64) __e_global_test.failure / total_perc;    \
		fprintf (stderr, "\n=======================================\n" \
		         "TEST SUMMARY:"                                       \
		         "\n - total:   %"E_FMT_USIZE                          \
			 "\n - \x1b[32msuccess: %"E_FMT_USIZE" (%.2f%%)\x1b[0m"\
			 "\n - \x1b[31mfailure: %"E_FMT_USIZE" (%.2f%%)\x1b[0m"\
		         "\n=======================================\n",        \
			 __e_global_test.success + __e_global_test.failure,    \
			 __e_global_test.success, succ_rate,                   \
			 __e_global_test.failure, fail_rate);                  \
		exit (__e_global_test.failure > 0 ? EXIT_FAILURE :             \
		      EXIT_SUCCESS);                                           \
	} while (0)

#if (E_STDC_VERSION >= E_STDC_VERSION_C23) && !E_CONFIG_TEST_TYPE_MACROS && !defined (E_CONFIG_TEST_TYPE_MACROS_OVERRIDE)

/**
 * Assert that a condition is truthy. The type is determined automatically.
 *
 * @param name: The name of the test
 * @param expr: The expression to test
 */
# define e_test_assert(name, expr) __e_test_assert (auto, name, expr)

/**
 * Assert that two values equal each other. The type is determined automatically.
 *
 * @param name: The name of the test
 * @param expr: The expression to check
 * @param check: The value it should have
 */
# define e_test_assert_eq(name, expr, check) __e_test_assert_eq (auto, typeof (result), name, expr, check)

#else /* (E_STDC_VERSION >= E_STDC_VERSION_C23) && !E_CONFIG_TEST_TYPE_MACROS */

/**
 * Assert that a condition is truthy.
 *
 * @param name: The name of the test
 * @param type: The type of \expr
 * @param expr: The expression to test
 */
#  define e_test_assert(name, type, expr) __e_test_assert (type, name, expr)

# if E_STDC_VERSION >= E_STDC_VERSION_C11

/**
 * Assert that two values equal each other.
 *
 * @param name: The name of the test
 * @param type: The type of \expr and \check
 * @param expr: The expression to check
 * @param check: The value it should have
 */
#  define e_test_assert_eq(name, type, expr, check) __e_test_assert_eq (type, type, name, expr, check)

# else /* E_STDC_VERSION >= E_STDC_VERSION_C11 */

/**
 * Assert that two values equal each other. Does not print the expected and got
 * value like in C11 and above due to limitations within the language.
 *
 * @param name: The name of the test
 * @param type: The type of \expr and \check
 * @param expr: The expression to check
 * @param check: The value it should have
 */
#  define e_test_assert_eq(name, type, expr, check) __e_test_assert (type, name, (expr) == (check))

# endif /* E_STDC_VERSION >= E_STDC_VERSION_C11 */
#endif /* (E_STDC_VERSION >= E_STDC_VERSION_C23) && !E_CONFIG_TEST_TYPE_MACROS */

#endif /* E_CONFIG_MODULE_TEST */
#endif /* _EMPOWER_TEST_H_ */
