#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_edit.h"
#include "game/terrain.h"

int Ov006_CountRiverOrBridgeNeighbors(
    int x,
    int y); /* neighbours that are river or bridge, not counting river mouths */
BOOL Ov006_IsWaterTile(int x,
                       int y); /* TRUE on sea, shoal, reef or a river mouth */

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])
#define TILE_AT(x, y) \
  (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* Map editor: TRUE when tile x, y counts as river. A river tile does; a bridge
 * does when it has a river or bridge neighbour, no open water at either end
 * of the river under it, and not two bridge neighbours along that river (a
 * vertical bridge crosses a river that runs left to right). The duplicated
 * cell compares, and the horizontal bridge check reading the cells above and
 * below x, y instead of around the neighbour, are as in the original. */
BOOL Ov006_CountsAsRiver(int x, int y) {
  int result = FALSE;
  int terrain = TILE_AT(x, y);
  int count;

  if (terrain == TERRAIN_BRIDGE) {
    if (Ov006_CountRiverOrBridgeNeighbors(x, y)) {
      result = TRUE;
    }
    if (CELL_AT(x, y) == MAP_CELL_BRIDGE_VERTICAL ||
        CELL_AT(x, y) == MAP_CELL_BRIDGE_VERTICAL) {
      if (x > 0 && Ov006_IsWaterTile(x - 1, y)) {
        result = FALSE;
      }
      if (x < STATE->unk_1c4->mapWidth - 1 && Ov006_IsWaterTile(x + 1, y)) {
        result = FALSE;
      }
      if (result) {
        count = 0;
        if (x > 0 && TILE_AT(x - 1, y) == TERRAIN_BRIDGE &&
            (CELL_AT(x - 1, y) == MAP_CELL_BRIDGE_VERTICAL ||
             CELL_AT(x - 1, y) == MAP_CELL_BRIDGE_VERTICAL)) {
          count++;
        }
        if (x < STATE->unk_1c4->mapWidth - 1 &&
            TILE_AT(x + 1, y) == TERRAIN_BRIDGE &&
            (CELL_AT(x + 1, y) == MAP_CELL_BRIDGE_VERTICAL ||
             CELL_AT(x + 1, y) == MAP_CELL_BRIDGE_VERTICAL)) {
          count++;
        }
        if (count >= 2) {
          result = FALSE;
        }
      }
    } else {
      if (y > 0 && Ov006_IsWaterTile(x, y - 1)) {
        result = FALSE;
      }
      if (y < STATE->unk_1c4->mapHeight - 1 && Ov006_IsWaterTile(x, y + 1)) {
        result = FALSE;
      }
      if (result) {
        count = 0;
        if (y > 0 && TILE_AT(x, y - 1) == TERRAIN_BRIDGE &&
            (CELL_AT(x, y - 1) == MAP_CELL_BRIDGE_HORIZONTAL ||
             CELL_AT(x, y + 1) == MAP_CELL_BRIDGE_HORIZONTAL)) {
          count++;
        }
        if (y < STATE->unk_1c4->mapHeight - 1 &&
            TILE_AT(x, y + 1) == TERRAIN_BRIDGE &&
            (CELL_AT(x, y - 1) == MAP_CELL_BRIDGE_HORIZONTAL ||
             CELL_AT(x, y + 1) == MAP_CELL_BRIDGE_HORIZONTAL)) {
          count++;
        }
        if (count >= 2) {
          result = FALSE;
        }
      }
    }
  }
  if (terrain == TERRAIN_RIVER || result) {
    return TRUE;
  }
  return FALSE;
}
