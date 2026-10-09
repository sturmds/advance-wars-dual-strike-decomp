#include "types.h"

extern u32 data_0229c88c;

extern int func_02117664(void (*callback)(void));
extern void func_02079348(void);

/* Returns 1 when the call returns 0; otherwise sets data_0229c88c to 1 and returns 0. */
BOOL func_0207930c(void)
{
    if (func_02117664(func_02079348) != 0) {
        data_0229c88c = 1;
        return 0;
    }
    return 1;
}
