#include "types.h"

BOOL func_020aac60(s16 *tile);
void func_020b2b70(int x, int y);

/* Looks for a target tile (func_020aac60) and, if there is one, moves the
 * acting unit there (func_020b2b70). */
void func_020aac28(void)
{
    s16 tile[2];

    if (func_020aac60(tile)) {
        func_020b2b70(tile[0], tile[1]);
    }
}
