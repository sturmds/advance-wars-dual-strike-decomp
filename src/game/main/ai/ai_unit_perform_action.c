#include "types.h"
#include "game/ai_unit_action.h"
#include "game/rng.h"
#include "game/unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

extern u32 gLinkPlay;

void AiUnit_AdjustAction(int x, int y, u8* action, u8* target, u8* arg);
void AiUnit_MarkFinishingCapture(int action);
void longjmp(void* p, int n);
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_TracePath(int x, int y, u8* out);
void PackNibblePairs(const u8* nibbles, u8* out);
void AiAction_DebugHook(int x, int y, int action, int target, int arg);
int Unit_GetFuelUsed(MapUnit* unit, int n);
void func_0208fda8(void);

/* Makes the active unit move to (x, y) and do action there, and records
 * the action in gUnitAction so the turn can show it. AiUnit_AdjustAction
 * may change action, target and arg first. */
void AiUnit_PerformAction(int x, int y, u8 action, u8 target, u8 arg) {
  MapUnit* unit;
  int fuelUsed;

  AiUnit_AdjustAction(x, y, &action, &target, &arg);
  AiUnit_MarkFinishingCapture(action);
  unit = GAME->actingUnit;
  if (x == unit->x && y == unit->y && action == 2) {
    unit->unk_08_7 = 1;
    longjmp(data_0229ea0c, 1);
  }
  RangeMap_FillUnitMoves(unit);
  RangeMap_TracePath(x, y, GAME->pathSteps);
  PackNibblePairs(GAME->pathSteps, gUnitAction.packed);
  AiAction_DebugHook(x, y, action, target, arg);

  gUnitAction.unitIndex = GAME->actingUnitIndex;
  gUnitAction.rngState = gRngState;
  gUnitAction.fromX = GAME->actingUnitStartX;
  gUnitAction.fromY = GAME->actingUnitStartY;
  gUnitAction.x = x;
  gUnitAction.y = y;
  gUnitAction.action = action;
  gUnitAction.target = target;
  gUnitAction.unk_07 = arg;
  fuelUsed = Unit_GetFuelUsed(GAME->actingUnit, GAME->unk_46);
  gUnitAction.fuelLeft = GAME->actingUnit->fuel - fuelUsed;
  gUnitAction.pending = TRUE;
  if (gLinkPlay != 0) {
    func_0208fda8();
  }
  longjmp(data_0229ea0c, 1);
}
