#include "types.h"

u8 func_ov005_0235d5bc(int index);
u8 func_ov005_0235d5cc(int index);

/* Table index of the entry whose bytes 2 and 3 are x and y (guessed as grid
 * position), or -1. */
int Ov005_FindCoListEntryAt(int x, int y)
{
    int i;

    for (i = 0; i < 33; i++) {
        if (x == func_ov005_0235d5cc(i) && y == func_ov005_0235d5bc(i)) {
            return i;
        }
    }
    return -1;
}
