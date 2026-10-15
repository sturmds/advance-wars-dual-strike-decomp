#include "types.h"
#include "game/ai_unit_action.h"
#include "game/rng.h"
#include "game/unit_action.h"
#include "game/Unk_02080440.h"
#include "game/ai_profile.h"
#include "game/math.h"
#include "game/ai_build_state.h"
#include "game/ai/ai_perform_action.h"
#include "game/ai/ai_count_units.h"
#include "game/ai/ai_unit_target.h"
#include "game/ai/ai_unit_claims.h"
#include "game/ai/ai_build_units.h"
#include "game/ai/ai_build_transport.h"
#include "game/ai.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

extern UnitAction data_0229d8c8;
extern u32 gLinkPlay;

void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void longjmp(void* p, int n);
void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_TracePath(int x, int y, u8* out);
void PackNibblePairs(const u8* nibbles, u8* out);
int Unit_GetFuelUsed(MapUnit* unit, int n);
void Link_SendPacket(UnitAction* action);
int Rng_Next(void);
int Unit_GetUsableMovement(MapUnit* unit);
int setjmp(void* p);

/* End-actions step 8. Tries five sub-steps in order and returns 1 at the first
 * one that returns 1 (needs another frame). Their purposes are not known. */
BOOL func_02090154(void) {
  data_021619c0->unk_08 = Ai_CountUnitsOfAiClass(1);
  if (AiTurn_TryBuildTCopter() == 1) {
    return 1;
  }
  if (AiTurn_TryBuildLander() == 1) {
    return 1;
  }
  if (AiTurn_TryBuildBlackBoatTransport() == 1) {
    return 1;
  }
  if (AiTurn_TryBuildBlackBoat() == 1) {
    return 1;
  }
  return AiTurn_TryBuildApc() == 1;
}

/* Runs gAiQueueRoutine for the next queue entry. A unit entry becomes
 * the active unit and is lifted off the unit map while the routine runs;
 * waited units are skipped. Entry 0 ends the queue and returns to the
 * phase step. If the routine made the unit act, the turn moves on to
 * show the action. */
void AiTurn_RunQueue(void) {
  u8 entry = *gAiQueueCursor;
  MapUnit* unit;
  MapTiles* map;
  Unk_02080440_48* game;

  if (entry == 0) {
    gAiTurnState->state = AI_STATE_RUN_PHASE;
    return;
  }
  if (entry != AI_QUEUE_RUN_ONCE) {
    MAP_STATE->actingUnitIndex = entry;
    MAP_STATE->actingUnit = &MAP_STATE->units[MAP_STATE->actingUnitIndex];
    gAiActiveUnitProfile = &gAiProfile->units[MAP_STATE->actingUnit->type - 1];
    unit = MAP_STATE->actingUnit;
    if ((unit->flags & MAP_UNIT_WAITED) || unit->type == 0) {
      gAiQueueCursor++;
      return;
    }
    MAP_STATE->actingUnitStartX = unit->x;
    MAP_STATE->actingUnitStartY = MAP_STATE->actingUnit->y;
    gAiTurnState->flags &= ~1;
    gAiTurnState->flags &= ~2;
    gAiTurnState->stepFlags = 0;
    gUnitAction.action = 0;
    gUnitAction.pending = 0;
    gUnitAction.target = 0;
    gUnitAction.unk_07 = 0;
    map = MAP_STATE->mapTiles;
    unit = MAP_STATE->actingUnit;
    map->unitAt[unit->x + map->rowStart[unit->y]] = 0;
    MAP_STATE->actingUnit->flags |= MAP_UNIT_LOCKED;
    gAiTurnState->roll = HwMod32(Rng_Next(), 100);
    gAiTurnState->unk_05 = Unit_GetUsableMovement(MAP_STATE->actingUnit);
  } else {
    gUnitAction.pending = 0;
  }

  if (setjmp(data_0229ea0c) == 0) {
    gAiQueueRoutine();
  }

  if (gUnitAction.pending == 0) {
    if (*gAiQueueCursor != AI_QUEUE_RUN_ONCE) {
      game = MAP_STATE;
      map = game->mapTiles;
      map->unitAt[game->actingUnit->x + map->rowStart[game->actingUnit->y]] =
          game->actingUnitIndex;
    }
    unit = MAP_STATE->actingUnit;
    if (unit != NULL) {
      unit->flags &= ~MAP_UNIT_LOCKED;
    }
    gAiQueueCursor++;
  } else {
    gAiQueueCursor++;
    gAiTurnState->state = AI_STATE_SHOW_ACTION;
  }
}

/* Copies gUnitAction to data_0229d8c8, except pending, and passes it to
 * Link_SendPacket. Purpose of the copy unknown. */
void func_0208fda8(void) {
  int i;

  data_0229d8c8.action = gUnitAction.action;
  data_0229d8c8.x = gUnitAction.x;
  data_0229d8c8.y = gUnitAction.y;
  data_0229d8c8.fromX = gUnitAction.fromX;
  data_0229d8c8.fromY = gUnitAction.fromY;
  data_0229d8c8.unitIndex = gUnitAction.unitIndex;
  data_0229d8c8.rngState = gUnitAction.rngState;
  data_0229d8c8.target = gUnitAction.target;
  data_0229d8c8.unk_07 = gUnitAction.unk_07;
  data_0229d8c8.fuelLeft = gUnitAction.fuelLeft;
  for (i = 0; i < (int)ARRAY_COUNT(data_0229d8c8.packed); i++) {
    data_0229d8c8.packed[i] = gUnitAction.packed[i];
  }
  Link_SendPacket(&data_0229d8c8);
}

/* Makes the active unit move to (x, y) and do action there, and records
 * the action in gUnitAction so the turn can show it. AiUnit_AdjustAction
 * may change action, target and arg first. */
void AiUnit_PerformAction(s16 x, s16 y, u8 action, u8 target, u8 arg) {
  MapUnit* unit;
  int fuelUsed;

  AiUnit_AdjustAction(x, y, &action, &target, &arg);
  AiUnit_MarkFinishingCapture(action);
  unit = MAP_STATE->actingUnit;
  if (x == unit->x && y == unit->y && action == 2) {
    unit->unk_08_7 = 1;
    longjmp(data_0229ea0c, 1);
  }
  RangeMap_FillUnitMoves(unit);
  RangeMap_TracePath(x, y, MAP_STATE->pathSteps);
  PackNibblePairs(MAP_STATE->pathSteps, gUnitAction.packed);
  AiAction_DebugHook(x, y, action, target, arg);

  gUnitAction.unitIndex = MAP_STATE->actingUnitIndex;
  gUnitAction.rngState = gRngState;
  gUnitAction.fromX = MAP_STATE->actingUnitStartX;
  gUnitAction.fromY = MAP_STATE->actingUnitStartY;
  gUnitAction.x = x;
  gUnitAction.y = y;
  gUnitAction.action = action;
  gUnitAction.target = target;
  gUnitAction.unk_07 = arg;
  fuelUsed = Unit_GetFuelUsed(MAP_STATE->actingUnit, MAP_STATE->unk_46);
  gUnitAction.fuelLeft = MAP_STATE->actingUnit->fuel - fuelUsed;
  gUnitAction.pending = TRUE;
  if (gLinkPlay != 0) {
    func_0208fda8();
  }
  longjmp(data_0229ea0c, 1);
}

/* Records an AI action that no unit takes in gUnitAction so the turn
 * can show it. Actions 15-18 are CO powers (the AI passes 16 after
 * CanUseCoPower and 17 after CanUseSuperPower); those are also copied
 * to data_0229d8c8 through UnitAction_Send when gLinkPlay is set. */
void AiPlayer_PerformAction(int action) {
  gUnitAction.action = action;
  gUnitAction.rngState = gRngState;
  gUnitAction.pending = TRUE;
  switch (action) {
    case 15:
    case 16:
    case 17:
    case 18:
      if (gLinkPlay != 0) {
        data_0229d8c8.rngState = gRngState;
        UnitAction_Send(action, 0, MAP_STATE->currentPlayer, 0);
      }
      break;
  }
  longjmp(data_0229ea0c, 1);
}
