#define E_TEST_IMPL
#include <e_test.h>

void test_alloc (void);
void test_arena (void);
void test_base64 (void);
void test_cobs (void);
void test_cobsr (void);
void test_cstr (void);
void test_debug (void);
void test_fs (void);
void test_ini (void);
void test_log (void);
void test_macro (void);
void test_mem (void);
void test_parse (void);
void test_rand (void);
void test_rbuf (void);
void test_result (void);
void test_str (void);
void test_types (void);
void test_vec (void);

int
main (int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	test_alloc ();
	test_arena ();
	test_base64 ();
	test_cobs ();
	test_cobsr ();
	test_cstr ();
	test_debug ();
	test_fs ();
	test_ini ();
	test_log ();
	test_macro ();
	test_mem ();
	test_parse ();
	test_rand ();
	test_rbuf ();
	test_result ();
	test_str ();
	test_types ();
	test_vec ();

	e_test_finish ();
}
