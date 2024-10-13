#include "convey.h"
#include <sys/stat.h>
#include <sys/types.h>

int
c_mkdir (const char *path, C_Permissions perm)
{
        return mkdir (path, (mode_t) perm);
}
