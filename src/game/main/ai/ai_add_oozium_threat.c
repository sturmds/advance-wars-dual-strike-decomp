#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_CanSeeUnitAt(u16 player, s16 x,
                         s16 y); /* fog, weather and terrain check of a tile */
BOOL UnitType_CanEnterTerrain(int unitType, u8 terrain);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* For an Oozium the current player can see (Player_CanSeeUnitAt), sets threatSum to
 * 255 on each of the four tiles next to it that UnitType_CanEnterTerrain accepts for an
 * Oozium. */
void Ai_AddOoziumThreat(MapUnit* unit) {
  int maxX;
  int y;
  int maxY;
  int x;
  int minX;
  int distance;

  if (unit->type != UNIT_OOZIUM) {
    return;
  }
  if (!Player_CanSeeUnitAt(STATE->currentPlayer, unit->x, unit->y)) {
    return;
  }
  /* Needed to match: minX and the first row clamp in different forms, and
     * the row loop starts from y itself (no minY), which sets the register
     * and stack slot choice. */
  minX = unit->x - 1;
  if (minX < 0) {
    minX = 0;
  }
  if (unit->y - 1 < 0) {
    y = 0;
  } else {
    y = unit->y - 1;
  }
  maxX = STATE->unk_1c4->mapWidth - 1;
  if (unit->x + 1 < maxX) {
    maxX = unit->x + 1;
  }
  maxY = STATE->unk_1c4->mapHeight - 1;
  if (unit->y + 1 < maxY) {
    maxY = unit->y + 1;
  }
  for (; y <= maxY; y++) {
    for (x = minX; x <= maxX; x++) {
      distance = 0;
      if (unit->x - x > 0) {
        distance += unit->x - x;
      } else {
        distance += x - unit->x;
      }
      if (unit->y - y > 0) {
        distance += unit->y - y;
      } else {
        distance += y - unit->y;
      }
      if (distance >= 1 && distance <= 1) {
        if (UnitType_CanEnterTerrain(
                UNIT_OOZIUM,
                MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE_INDEX(x, y)]))) {
          STATE->mapTiles->threatSum[TILE_INDEX(x, y)] = 255;
        }
      }
    }
  }
}
