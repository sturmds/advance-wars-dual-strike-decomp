#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

int UnitType_GetAttackKind(int unitType);
int AiUnit_PickIndirectTarget(int x, int y);
int AiUnit_FindBestAdjacentTarget(int x, int y);

/* AiUnit_AdjustAction calls this for action 2 (wait) at (x, y). If a
 * target can be hit from there, the action becomes an attack on its unit
 * slot. Direct units check any tile; indirect units only if they stay on
 * the tile they stand on. */
void AiUnit_WaitToAttack(int x, int y, u8 *action, u8 *target)
{
    int slot;

    if (UnitType_GetAttackKind(GAME->actingUnit->type) == 0) {
        slot = AiUnit_FindBestAdjacentTarget(x, y);
        if (slot != 0) {
            *action = AI_ACTION_ATTACK_UNIT;
            *target = slot;
        }
    } else if (UnitType_GetAttackKind(GAME->actingUnit->type) == 1) {
        if (x == GAME->actingUnit->x && y == GAME->actingUnit->y) {
            slot = AiUnit_PickIndirectTarget(x, y);
            if (slot != 0) {
                *action = AI_ACTION_ATTACK_UNIT;
                *target = slot;
            }
        }
    }
}
