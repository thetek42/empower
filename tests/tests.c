#define E_TEST_IMPL
#include <e_test.h>

extern void test_alloc (void);
extern void test_arena (void);
extern void test_base64 (void);
extern void test_bitvec (void);
extern void test_cobs (void);
extern void test_cobsr (void);
extern void test_cstr (void);
extern void test_debug (void);
extern void test_ini (void);
extern void test_log (void);
extern void test_macro (void);
extern void test_mem (void);
extern void test_rand (void);
extern void test_stdc (void);

int
main (void)
{
        test_alloc ();
        test_arena ();
        test_base64 ();
        test_bitvec ();
        test_cobs ();
        test_cobsr ();
        test_cstr ();
        test_debug ();
        test_ini ();
        test_log ();
        test_macro ();
        test_mem ();
        test_rand ();
        test_stdc ();

        e_test_finish ();
        return 0;
}
