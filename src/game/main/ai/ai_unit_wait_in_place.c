#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiUnit_RetreatToRepair(void);
void AiUnit_MoveToSafestTile(void);
void func_020b81b0(void);
void Ai_ClearActingUnitTarget(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Last step of the AI order routines: the acting unit stays on its tile.
 * It first tries MoveToSafestTile with terrain ignored. */
void AiUnit_WaitInPlace(void) {
  gAiTurnState->stepFlags |= AI_STEP_IGNORE_TERRAIN;
  AiUnit_RetreatToRepair();
  AiUnit_MoveToSafestTile();
  func_020b81b0();
  Ai_ClearActingUnitTarget();
  AiUnit_PerformAction(STATE->actingUnit->x, STATE->actingUnit->y, 2, 0, 0);
}
