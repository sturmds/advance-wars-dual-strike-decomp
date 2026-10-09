#include "types.h"

BOOL func_020aa0a0(s16 *tile);
void AiUnit_MoveTowardTile(int x, int y);

/* Looks for a target tile (func_020aa0a0) and, if there is one, moves the
 * acting unit there (AiUnit_MoveTowardTile). */
void func_020aa068(void)
{
    s16 tile[2];

    if (func_020aa0a0(tile)) {
        AiUnit_MoveTowardTile(tile[0], tile[1]);
    }
}
