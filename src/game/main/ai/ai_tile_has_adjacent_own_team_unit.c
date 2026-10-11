#include "types.h"

int func_02096e28(int x, int y);

/* 1 when a unit of the current player's team stands on a tile next to (x, y). */
int func_02096d98(int x, int y)
{
    if (func_02096e28(x - 1, y) == 1) {
        return 1;
    }
    if (func_02096e28(x + 1, y) == 1) {
        return 1;
    }
    if (func_02096e28(x, y - 1) == 1) {
        return 1;
    }
    return func_02096e28(x, y + 1) == 1;
}
