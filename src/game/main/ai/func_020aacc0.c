#include "types.h"

BOOL func_020aacf8(s16 *tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (func_020aacf8) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aacc0(void)
{
    s16 tile[2];

    if (func_020aacf8(tile)) {
        AiUnit_MoveTowardTile(tile[0], tile[1]);
    }
}
