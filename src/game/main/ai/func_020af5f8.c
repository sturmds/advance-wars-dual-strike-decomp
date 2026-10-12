#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
void AiUnit_GoToSupply(void);
void AiUnit_HeadForRepairTile(void);
void AiUnit_JoinDamagedUnit(void);
void AiUnit_LaunchSilo(void);
void AiStep_DebugHook(void);

/* AI step shared by AiFoot_MoveAndAttack and many queue routines. When the
 * acting unit's serviceWanted is 1 or 2, runs AiUnit_GoToSupply or AiUnit_HeadForRepairTile;
 * infantry and mechs first get AiUnit_LaunchSilo. */
void func_020af5f8(void) {
  if (!Unit_IsBlackHoleOnFlaggedTile(GAME->actingUnit, GAME->actingUnit->x,
                                     GAME->actingUnit->y)) {
    AiUnit_JoinDamagedUnit();
  }
  if (GAME->actingUnit->serviceWanted == 0) {
    return;
  }
  if (GAME->actingUnit->type <= UNIT_MECH) {
    AiUnit_LaunchSilo();
  }
  gAiTurnState->flags |= 2;
  AiStep_DebugHook();
  switch (GAME->actingUnit->serviceWanted) {
    case 1:
      AiUnit_GoToSupply();
      break;
    case 2:
      AiUnit_HeadForRepairTile();
      break;
  }
}
