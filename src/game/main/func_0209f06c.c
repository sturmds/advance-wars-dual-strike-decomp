#include "types.h"

extern u8 gTerrainIsProperty[32];

BOOL func_0209f06c(int index)
{
    return gTerrainIsProperty[index & 31] != 0;
}
