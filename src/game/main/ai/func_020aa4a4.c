#include "types.h"

BOOL func_020aa4dc(s16 *tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (func_020aa4dc) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa4a4(void)
{
    s16 tile[2];

    if (func_020aa4dc(tile)) {
        AiUnit_MoveTowardTile(tile[0], tile[1]);
    }
}
