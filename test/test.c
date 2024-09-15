#define E_TEST_MAIN
#define E_TEST_TYPE_MACROS
#include "empower.h"

int
main (int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	e_test_assert ("test", int, 42 == 42);

	e_test_finish ();
}
