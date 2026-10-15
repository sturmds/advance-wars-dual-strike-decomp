#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

int Ov006_GetUnitCodeAt(int x, int y);
int Ov006_RemoveUnitAt(int withEffect, int x, int y);

#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* After tile x, y became sea: removes a unit there if its movement type
 * cannot enter the tile (movement chart cost -1). For an empty tile
 * Ov006_GetUnitCodeAt returns UNIT_TYPE_COUNT, which is still above 0, so
 * the stats row read for it lies past the table. */
void Ov006_RemoveUnitOnImpassableTile(int x, int y) {
  int unit = Ov006_GetUnitCodeAt(x, y);
  int terrain;

  if (unit <= 0) {
    return;
  }
  terrain = MAP_TILE_TERRAIN(
      MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]]);
  if (CHART[Ov000_gUnitStats[unit & 63].moveType][terrain] == -1) {
    Ov006_RemoveUnitAt(0, x, y);
  }
}
