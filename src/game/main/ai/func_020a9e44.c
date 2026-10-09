#include "types.h"

BOOL func_020a9e7c(s16 *tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (func_020a9e7c) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020a9e44(void)
{
    s16 tile[2];

    if (func_020a9e7c(tile)) {
        AiUnit_MoveTowardTile(tile[0], tile[1]);
    }
}
