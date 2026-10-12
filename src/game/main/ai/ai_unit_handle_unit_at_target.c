#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiUnit_PerformAction(int x, int y, u8 action, u8 target, u8 arg);
int UnitType_GetAttackKind(int unitType);
void AiUnit_AttackFromPlaceThenMove(MapUnit* unit);
void AiUnit_PickDirectAttackInReachOf(MapUnit* unit);

/* Puts unit fromSlot back on its tile and makes toSlot the acting unit:
 * lifts it off its tile, records its start position and rolls a new
 * gAiTurnState->roll. */
void Ai_SwitchActingUnit(int fromSlot, int toSlot);

/* AiUnit_AdjustAction calls this when the acting unit is about to do action 10
 * on (x, y), where another unit stands. Nothing happens if that unit has
 * waited or has AI order 7. Otherwise, if it is capturing or has more hp
 * than the acting unit, it becomes the acting unit for a moment: it goes
 * on capturing (action 3) and the attack planner for the original
 * unit's attack kind runs. If not, its slot is written at
 * gAiQueueCursor and the attack planner runs for it. */
void AiUnit_HandleUnitAtTarget(int x, int y) {
  MapUnit* other;
  MapUnit* acting;
  u8 slot;
  u8 actingIndex;
  Unk_02080440_48* game = data_027c027c->unk_48[data_027c0284];
  MapTiles* tiles = game->mapTiles;

  slot = tiles->unitAt[tiles->rowStart[y] + x];
  other = &game->units[slot];
  acting = game->actingUnit;
  actingIndex = game->actingUnitIndex;

  if (other->flags & MAP_UNIT_WAITED) {
    return;
  }
  if (other->aiOrder == 7) {
    return;
  }
  gAiTurnState->stepFlags |= AI_STEP_UNIT_AT_TARGET;
  if (other->captureProgress != 0 || acting->hp < other->hp) {
    Ai_SwitchActingUnit(actingIndex, slot);
    gAiQueueCursor--;
    if (other->captureProgress != 0) {
      AiUnit_PerformAction(other->x, other->y, 3, 0, 0);
    }
    /* 0: direct, 1: indirect */
    switch (UnitType_GetAttackKind(acting->type)) {
      case 0:
        AiUnit_PickDirectAttackInReachOf(acting);
        break;
      case 1:
        AiUnit_AttackFromPlaceThenMove(acting);
        break;
    }
    Ai_SwitchActingUnit(slot, actingIndex);
    gAiQueueCursor++;
  } else {
    *gAiQueueCursor = slot;
    gAiQueueCursor--;
    switch (UnitType_GetAttackKind(other->type)) {
      case 0:
        AiUnit_PickDirectAttackInReachOf(other);
        break;
      case 1:
        AiUnit_AttackFromPlaceThenMove(other);
        break;
    }
    gAiQueueCursor++;
  }
}
