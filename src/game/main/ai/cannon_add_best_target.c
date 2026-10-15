#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/unit_stats.h"
#include "game/range_map.h"
#include "game/ai/ai_best_damage.h"
#include "game/ai.h"

/* needed to match: the original reads the slot again for each use. */
#define UNIT_SLOT_AT(x, y) \
  (MAP_STATE->mapTiles->unitAt[(x) + MAP_STATE->mapTiles->rowStart[(y)]])

void RangeMap_FillCone(u16 x, u16 y, u16 variant, u16 size, int value);
BOOL Unit_IsVisibleToCurrentPlayer(u16 unitSlot);
int DamageFromCannons(int playerIndex, int coSlot);

/* Picks the target of a cannon object (kind, variant) at (x, y): the visible
 * enemy unit in its range (marked by RangeMap_FillCone) worth the most, hp times
 * cost / 100, with ties going to the later tile. Adds it to the AI target
 * list with damage less the target owner's DamageFromCannons reduction, or
 * empties the list when no unit is in range. */
void Cannon_AddBestTarget(int x, int y, int damage, int kind, int variant) {
  MapUnit* unit;
  u32 bestValue;
  u32 value;
  u16 bestX;
  u16 bestY;
  u8 bestSlot;
  u8 slot;
  int tx;
  int ty;
  int dx;
  int reduction;

  bestValue = 0;
  bestX = 0;
  bestY = 0;
  bestSlot = 0;
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  RangeMap_Fill(-1);
  dx = MapObject_GetShotOriginX(kind, variant);
  RangeMap_FillCone(x + dx, y + MapObject_GetShotOriginY(kind, variant),
                    variant, gMapObjectConeLengths[kind], 0);

  for (ty = 0; ty < MAP_STATE->unk_1c4->mapHeight; ty++) {
    for (tx = 0; tx < MAP_STATE->unk_1c4->mapWidth; tx++) {
      if (MAP_STATE->rangeMap[ty][tx] < 0) {
        continue;
      }
      slot = UNIT_SLOT_AT(tx, ty);
      if (slot == 0) {
        continue;
      }
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
          MAP_STATE->players[(slot >> 6) + 1].team) {
        continue;
      }
      if (!Player_CanSeeUnitAt(MAP_STATE->currentPlayer, tx, ty)) {
        continue;
      }
      unit = &MAP_STATE->units[UNIT_SLOT_AT(tx, ty)];
      if (unit->flags & MAP_UNIT_DIVED) {
        continue;
      }
      if (!Unit_IsVisibleToCurrentPlayer(UNIT_SLOT_AT(tx, ty))) {
        continue;
      }
      value = unit->hp * (Ov000_gUnitStats[unit->type].costDiv10 / 10);
      if (bestValue <= value) {
        bestX = tx;
        bestValue = value;
        bestY = ty;
        bestSlot = UNIT_SLOT_AT(tx, ty);
      }
    }
  }

  if (bestSlot == 0) {
    AiTargetList_Clear();
    return;
  }
  reduction = 0;
  reduction += DamageFromCannons(
      MAP_UNIT_OWNER(MAP_STATE, &MAP_STATE->units[bestSlot]), 0);
  AiTargetList_Add(bestX, bestY, bestSlot, damage - reduction);
}
