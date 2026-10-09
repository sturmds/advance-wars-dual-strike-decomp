#include "types.h"

int AiActingUnit_GetSearchMovement(void);
void AiActingUnit_MarkAttackRange(int movement);
BOOL func_020a870c(s16 *tile);
BOOL AiUnit_FindFiringTile(s16 *tile);

/* Runs the two setup passes, then two tile searches in turn. Writes the
 * tile to out only when both succeed. */
BOOL func_020aac60(s16 *out)
{
    s16 tile[2];

    AiActingUnit_MarkAttackRange(AiActingUnit_GetSearchMovement());
    if (!func_020a870c(tile)) {
        return FALSE;
    }
    if (!AiUnit_FindFiringTile(tile)) {
        return FALSE;
    }
    out[0] = tile[0];
    out[1] = tile[1];
    return TRUE;
}
