#include "types.h"

BOOL func_020aa9fc(s16 *tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (func_020aa9fc) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa9c4(void)
{
    s16 tile[2];

    if (func_020aa9fc(tile)) {
        AiUnit_MoveTowardTile(tile[0], tile[1]);
    }
}
