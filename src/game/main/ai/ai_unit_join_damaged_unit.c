#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y);
void AiUnit_LaunchSilo(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* A unit at 50 hp or less that carries nothing joins a unit of its own type
 * it can reach: not carried or carrying, at most 90 hp, hp sum at most 100,
 * and Unit_IsBlackHoleOnFlaggedTile says no. Picks the one with the highest
 * hp, the last on ties. Infantry and mechs try AiUnit_LaunchSilo first. */
void AiUnit_JoinDamagedUnit(void) {
  int bestX;
  int bestY;
  int bestHp;
  int slot;
  MapUnit* unit;

  bestX = -1;
  bestY = bestX;
  bestHp = 0;
  if (STATE->actingUnit->hp > 50) {
    return;
  }
  if (STATE->actingUnit->cargo[0] != 0) {
    return;
  }
  RangeMap_FillUnitMoves(STATE->actingUnit);
  for (slot = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; slot++) {
    unit = &STATE->units[slot];
    if (unit->type == UNIT_NONE) {
      continue;
    }
    if (STATE->rangeMap[unit->y][unit->x] < 0) {
      continue;
    }
    if (STATE->actingUnit->type != unit->type) {
      continue;
    }
    if (STATE->actingUnit == unit) {
      continue;
    }
    if (unit->hp >= 91) {
      continue;
    }
    if (Unit_IsBlackHoleOnFlaggedTile(unit, unit->x, unit->y) == 1) {
      continue;
    }
    if (STATE->actingUnit->hp + unit->hp > 100) {
      continue;
    }
    if (unit->cargo[0] != 0) {
      continue;
    }
    if (unit->flags & MAP_UNIT_CARRIED) {
      continue;
    }
    if (unit->hp >= bestHp) {
      bestX = unit->x;
      bestY = unit->y;
      bestHp = unit->hp;
    }
  }
  if (bestX == -1) {
    return;
  }
  if (STATE->actingUnit->type <= UNIT_MECH) {
    AiUnit_LaunchSilo();
  }
  STATE->actingUnit->serviceWanted = 0;
  AiUnit_PerformAction(bestX, bestY, AI_ACTION_JOIN, 0, 0);
}
