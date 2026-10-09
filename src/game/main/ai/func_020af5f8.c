#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit *unit, int x, int y);
void func_020af700(void);
void AiUnit_HeadForRepairTile(void);
void func_020aff0c(void);
void AiUnit_LaunchSilo(void);
void func_020b81ac(void);

/* AI step shared by AiFoot_MoveAndAttack and many queue routines. When the
 * acting unit's unk_0a_12 is 1 or 2, runs func_020af700 or AiUnit_HeadForRepairTile;
 * infantry and mechs first get AiUnit_LaunchSilo. */
void func_020af5f8(void)
{
    if (!Unit_IsBlackHoleOnFlaggedTile(GAME->actingUnit, GAME->actingUnit->x, GAME->actingUnit->y)) {
        func_020aff0c();
    }
    if (GAME->actingUnit->unk_0a_12 == 0) {
        return;
    }
    if (GAME->actingUnit->type <= UNIT_MECH) {
        AiUnit_LaunchSilo();
    }
    gAiTurnState->flags |= 2;
    func_020b81ac();
    switch (GAME->actingUnit->unk_0a_12) {
    case 1:
        func_020af700();
        break;
    case 2:
        AiUnit_HeadForRepairTile();
        break;
    }
}
