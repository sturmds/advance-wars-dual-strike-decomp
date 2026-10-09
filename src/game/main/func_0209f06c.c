#include "types.h"

extern u8 data_02161a04[32];

BOOL func_0209f06c(int index)
{
    return data_02161a04[index & 31] != 0;
}
