#include "types.h"

BOOL func_020aa0a0(s16 *tile);
void func_020b2b70(int x, int y);

/* Looks for a target tile (func_020aa0a0) and, if there is one, moves the
 * acting unit there (func_020b2b70). */
void func_020aa068(void)
{
    s16 tile[2];

    if (func_020aa0a0(tile)) {
        func_020b2b70(tile[0], tile[1]);
    }
}
