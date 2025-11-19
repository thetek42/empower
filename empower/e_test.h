#ifndef EMPOWER_TEST_H_
#define EMPOWER_TEST_H_

/*******************************************************************************
 *
 * This module provides basic functionality for writing unit tests.
 *
 ******************************************************************************/

#include <stddef.h>
#include <stdio.h>

/* (scroll down to get to the important stuff, these are just some helpers) */

#ifndef E_STRINGIFY
# define E_STRINGIFY_HELPER_(x) #x
# define E_STRINGIFY(x) E_STRINGIFY_HELPER_ (x)
#endif
#ifndef E_SOURCE_LOCATION
# define E_SOURCE_LOCATION __FILE__ ":" E_STRINGIFY (__LINE__)
#endif

#if defined (__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
# ifndef E_AUTO_FMT
#  define E_AUTO_FMT(value) _Generic ((value), \
        char: "%c", \
        signed char: "%d", \
        signed short: "%d", \
        signed int: "%d", \
        signed long: "%ld", \
        signed long long: "%lld", \
        unsigned char: "%u", \
        unsigned short: "%u", \
        unsigned int: "%u", \
        unsigned long: "%lu", \
        unsigned long long: "%llu", \
        float: "%f", \
        double: "%g", \
        long double: "%lg", \
        _Bool: "%d", \
        default: "%p")
# endif
# define E_TEST_ASSERT_EQ_PRINT_VALUES(expr, result, other) \
	do { \
		fprintf (stderr, "      - Expression `%s` returned ", #expr); \
		fprintf (stderr, E_AUTO_FMT (result), result); \
		fprintf (stderr, " but expected "); \
		fprintf (stderr, E_AUTO_FMT (other), other); \
		fprintf (stderr, "\n"); \
	} while (0)
#else
# define E_TEST_ASSERT_EQ_PRINT_VALUES(expr, result, other) \
	fprintf (stderr, "     - Expression `%s` returned unexpected value\n", #expr)
#endif

/* public interface **********************************************************/

#define e_test_assert(msg, expr) \
        e_test_assert_ ((msg), E_SOURCE_LOCATION, #expr, (expr))

#define e_test_assert_mem_eq(msg, expr, check, len) \
        e_test_assert_mem_eq_ ((msg), E_SOURCE_LOCATION, #expr, (expr), (check), (len))

#define e_test_assert_ptr_eq(msg, expr, check) \
        e_test_assert_ptr_eq_ ((msg), E_SOURCE_LOCATION, #expr, (expr), (check))

#define e_test_assert_null(msg, expr) \
        e_test_assert_null_ ((msg), E_SOURCE_LOCATION, #expr, (expr))

#define e_test_assert_str_eq(msg, expr, check) \
        e_test_assert_str_eq_ ((msg), E_SOURCE_LOCATION, #expr, (expr), (check))

#define e_test_assert_ptr_aligned(msg, expr, alignment) \
        e_test_assert_ptr_aligned_ ((msg), E_SOURCE_LOCATION, #expr, (expr), (alignment))

#define e_test_assert_eq(msg, type, expr, check) \
        do { \
                type result_ = (expr); \
                type other_ = (check); \
                if (result_ == other_) { \
                        e_test_success_counter_ += 1; \
                } else { \
                        e_test_failure_counter_ += 1; \
                        e_test_print_fail_msg_ ("e_test_assert_eq", (msg), E_SOURCE_LOCATION); \
                        E_TEST_ASSERT_EQ_PRINT_VALUES (expr, result_, other_); \
                } \
        } while (0)

void e_test_finish (void);

/* implementation details *****************************************************/

extern unsigned long e_test_success_counter_;
extern unsigned long e_test_failure_counter_;

void e_test_print_fail_msg_ (const char *test_fn,
                             const char *msg,
                             const char *location);

void e_test_assert_ (const char *msg,
                     const char *location,
                     const char *expr,
                     int value);

void e_test_assert_mem_eq_ (const char *msg,
                            const char *location,
                            const char *expr,
                            const void *got,
                            const void *want,
                            size_t len);

void e_test_assert_ptr_eq_ (const char *msg,
                            const char *location,
                            const char *expr,
                            const void *got,
                            const void *want);

void e_test_assert_null_ (const char *msg,
                          const char *location,
                          const char *expr,
                          const void *ptr);

void e_test_assert_str_eq_ (const char *msg,
                            const char *location,
                            const char *expr,
                            const char *got,
                            const char *want);

void e_test_assert_ptr_aligned_ (const char *msg,
                                 const char *location,
                                 const char *expr,
                                 const void *got,
                                 size_t alignment);

/******************************************************************************/

#ifdef E_TEST_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if __STDC_VERSION__ >= 199901L
# include <stdint.h>
#endif

unsigned long e_test_success_counter_ = 0;
unsigned long e_test_failure_counter_ = 0;

void
e_test_finish (void)
{
        unsigned long total;
        double percentage;
        int color;

        total = e_test_success_counter_ + e_test_failure_counter_;
        percentage = ((double) e_test_success_counter_ / (double) total) * 100.0;
        color = e_test_failure_counter_ == 0 ? 32 : 31;

        fprintf (stderr, "\x1b[%dmDONE\x1b[0m  Executed %lu tests\n", color, total);
        fprintf (stderr, "      - Successful: %lu\n", e_test_success_counter_);
        fprintf (stderr, "      - Failures: %lu\n", e_test_failure_counter_);
        fprintf (stderr, "      - Success percentage: %.2f%%\n", percentage);
        exit (e_test_failure_counter_ == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

void
e_test_print_fail_msg_ (const char *test_fn,
                        const char *msg,
                        const char *location)
{
        fprintf (stderr, "\x1b[31mFAIL\x1b[0m  %s\n", test_fn);
        fprintf (stderr, "      - Message: %s\n", msg);
        fprintf (stderr, "      - Location: %s\n", location);
}

void
e_test_assert_ (const char *msg,
                const char *location,
                const char *expr,
                int value)
{
        if (value) {
                e_test_success_counter_ += 1;
        } else {
                e_test_failure_counter_ += 1;
                e_test_print_fail_msg_ ("e_test_assert", msg, location);
                fprintf (stderr, "      - Expression `%s` returned 0\n", expr);
        }
}

void
e_test_assert_mem_eq_ (const char *msg,
                       const char *location,
                       const char *expr,
                       const void *got,
                       const void *want,
                       size_t len)
{
        const unsigned char *uwant = want;
        const unsigned char *ugot = got;
        int matches = 1;
        size_t i;

        if (ugot == NULL) {
                e_test_failure_counter_ += 1;
                e_test_print_fail_msg_ ("e_test_assert_mem_eq", msg, location);
                fprintf (stderr, "      - Expression `%s` returned NULL\n", expr);
                return;
        }

        for (i = 0; i < len; i++) {
                if (ugot[i] != uwant[i]) {
                        matches = 0;
                        break;
                }
        }

        if (matches) {
                e_test_success_counter_ += 1;
        } else {
                e_test_failure_counter_ += 1;
                e_test_print_fail_msg_ ("e_test_assert_mem_eq", msg, location);
                fprintf (stderr, "      - Expression `%s` returned pointer `%p`\n", expr, got);
                fprintf (stderr, "      - The memory of `%p` does not equal the expected memory at `%p`\n", got, want);
                fprintf (stderr, "      - The mismatch occured after %lu bytes\n", (unsigned long) i);
        }
}

void
e_test_assert_ptr_eq_ (const char *msg,
                       const char *location,
                       const char *expr,
                       const void *got,
                       const void *want)
{
        ptrdiff_t diff;
        if (got == want) {
                e_test_success_counter_ += 1;
        } else {
                e_test_failure_counter_ += 1;
                diff = (const unsigned char *) got - (const unsigned char *) want;
                e_test_print_fail_msg_ ("e_test_assert_ptr_eq", msg, location);
                fprintf (stderr, "      - Expression `%s` returned pointer \"%p\"\n", expr, got);
                fprintf (stderr, "      - The pointer %p does not equal the expected pointer \"%p\"\n", got, want);
                fprintf (stderr, "      - Difference: (got - want) = %ld\n", (long) diff);
        }
}

void
e_test_assert_null_ (const char *msg,
                     const char *location,
                     const char *expr,
                     const void *ptr)
{
        if (ptr == NULL) {
                e_test_success_counter_ += 1;
        } else {
                e_test_failure_counter_ += 1;
                e_test_print_fail_msg_ ("e_test_assert_null", msg, location);
                fprintf (stderr, "      - Expression `%s` returned pointer \"%p\"\n", expr, ptr);
                fprintf (stderr, "      - Expected a null pointer\n");
        }
}

void
e_test_assert_str_eq_ (const char *msg,
                       const char *location,
                       const char *expr,
                       const char *got,
                       const char *want)
{
        if (got == NULL) {
                e_test_failure_counter_ += 1;
                e_test_print_fail_msg_ ("e_test_assert_str_eq", msg, location);
                fprintf (stderr, "      - Expression `%s` returned NULL\n", expr);
                return;
        }

        if (strcmp (got, want) == 0) {
                e_test_success_counter_ += 1;
        } else {
                e_test_failure_counter_ += 1;
                e_test_print_fail_msg_ ("e_test_assert_str_eq", msg, location);
                fprintf (stderr, "      - Expression `%s` returned string \"%s\"\n", expr, got);
                fprintf (stderr, "      - The returned string does not equal the expected string \"%s\"\n", want);
        }
}

void
e_test_assert_ptr_aligned_ (const char *msg,
                            const char *location,
                            const char *expr,
                            const void *got,
                            size_t alignment)
{
        int is_aligned;

        if (got == NULL) {
                e_test_failure_counter_ += 1;
                e_test_print_fail_msg_ ("e_test_assert_ptr_aligned", msg, location);
                fprintf (stderr, "      - Expression `%s` returned NULL\n", expr);
                return;
        }

#if __STDC_VERSION__ >= 199901L
	is_aligned = ((uintptr_t) got % alignment) == 0;
#else
	is_aligned = ((size_t) got % alignment) == 0;
#endif

        if (is_aligned) {
                e_test_success_counter_ += 1;
        } else {
                e_test_failure_counter_ += 1;
                e_test_print_fail_msg_ ("e_test_assert_ptr_aligned", msg, location);
                fprintf (stderr, "      - Expression `%s` returned pointer \"%p\"\n", expr, got);
                fprintf (stderr, "      - The returned pointer is not aligned to \"%u\" bytes\n", (unsigned) alignment);
        }
}

#endif /* E_TEST_IMPL */

#endif /* EMPOWER_TEST_H_ */
