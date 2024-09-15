#include "empower.h"

int
main (int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	e_log_debug ("debug");
	e_log_info ("info");
	e_log_warn ("warn");
	e_log_error ("error");

	int x = 42;
	e_debug (x);
}
