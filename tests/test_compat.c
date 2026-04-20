#include "e_compat.h"

#include <stddef.h> /* -Wpedantic is fun */

void
test_compat (void)
{
    int x = 42;
    E_TYPEOF (x) y = 69;
    x = y;
}
