#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int Map_GetDistance(int x1, int y1, s16 x2, s16 y2);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

/* TRUE when a unit of the current player is already capturing the tile (x, y),
 * or an Infantry or Mech whose AI target is the tile can reach it: Infantry
 * within 3 moves, Mech also within 2 tiles of it. Uses MapTiles.rangeScratch
 * for the Infantry move costs and restores the default range map. */
BOOL Ai_IsCaptureSiteTaken(int x, int y) {
  int maxX;
  int maxY;
  int cy;
  int minX;
  int cx;
  int distance;
  int slot;
  MapUnit* unit;

  RangeMap_SetBuffer(MAP->rangeScratch);
  RangeMap_FillMoves(x, y, UNIT_INFANTRY, 3, 1);
  slot = MAP->unitAt[TILE_INDEX(x, y)];
  if (slot != 0 && Unit_IsCurrentPlayers(slot) == TRUE &&
      STATE->units[slot].captureProgress != 0) {
    RangeMap_SetBuffer(MAP->rangeMapTiles);
    return TRUE;
  }
  minX = x - 3;
  if (minX < 0) {
    minX = 0;
  }
  cy = y - 3;
  if (cy < 0) {
    cy = 0;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (x + 3 < maxX) {
    maxX = x + 3;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (y + 3 < maxY) {
    maxY = y + 3;
  }
  for (; cy <= maxY; cy++) {
    for (cx = minX; cx <= maxX; cx++) {
      distance = 0;
      if (x - cx > 0) {
        distance += x - cx;
      } else {
        distance += cx - x;
      }
      if (y - cy > 0) {
        distance += y - cy;
      } else {
        distance += cy - y;
      }
      if (distance < 0 || distance > 3) {
        continue;
      }
      if (STATE->rangeMap[cy][cx] < 0) {
        continue;
      }
      unit = &STATE->units[MAP->unitAt[TILE_INDEX(cx, cy)]];
      if (unit->type == 0 || unit->type > UNIT_MECH) {
        continue;
      }
      if (unit->aiTargetX != x || unit->aiTargetY != y) {
        continue;
      }
      if (unit->type == UNIT_MECH &&
          Map_GetDistance(unit->x, unit->y, x, y) > 2) {
        continue;
      }
      RangeMap_SetBuffer(MAP->rangeMapTiles);
      return TRUE;
    }
  }
  RangeMap_SetBuffer(MAP->rangeMapTiles);
  return FALSE;
}
