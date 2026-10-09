#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit *unit, int x, int y);
void AiUnit_GoToSupply(void);
void func_020aff0c(void);
void AiUnit_LaunchSilo(void);
void func_020b81ac(void);

/* Like func_020af5f8, but only runs AiUnit_GoToSupply and only when the acting
 * unit's unk_0a_12 is 1. */
void AiUnit_SeekSupply(void)
{
    if (!Unit_IsBlackHoleOnFlaggedTile(GAME->actingUnit, GAME->actingUnit->x, GAME->actingUnit->y)) {
        func_020aff0c();
    }
    if (GAME->actingUnit->unk_0a_12 != 1) {
        return;
    }
    if (GAME->actingUnit->type <= UNIT_MECH) {
        AiUnit_LaunchSilo();
    }
    gAiTurnState->flags |= 2;
    func_020b81ac();
    AiUnit_GoToSupply();
}
