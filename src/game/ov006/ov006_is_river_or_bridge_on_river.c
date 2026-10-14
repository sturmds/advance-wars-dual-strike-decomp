#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_CountRiverNeighbors(
    int x, int y); /* river neighbours, not counting river mouths */
BOOL Ov006_IsWaterTile(int x,
                       int y); /* TRUE on sea, shoal, reef or a river mouth */

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])
#define TILE_AT(x, y) \
  (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* Bridge cell code that checks its left and right neighbours; the name
 * guesses the direction from that. */
#define CELL_BRIDGE_HORIZONTAL 22

/* Map editor: TRUE when tile x, y is river, or a bridge with a river
 * neighbour, no open water at either end, and not two bridge neighbours along
 * its direction. func_ov006_02360308 builds a neighbour mask from it. The
 * duplicated cell compare is as in the original. */
BOOL Ov006_IsRiverOrBridgeOnRiver(int x, int y) {
  int result = FALSE;
  int terrain = TILE_AT(x, y);
  int count;

  if (terrain == TERRAIN_BRIDGE) {
    if (Ov006_CountRiverNeighbors(x, y)) {
      result = TRUE;
    }
    if (CELL_AT(x, y) == CELL_BRIDGE_HORIZONTAL ||
        CELL_AT(x, y) == CELL_BRIDGE_HORIZONTAL) {
      if (x > 0 && Ov006_IsWaterTile(x - 1, y)) {
        result = FALSE;
      }
      if (x < STATE->unk_1c4->mapWidth - 1 && Ov006_IsWaterTile(x + 1, y)) {
        result = FALSE;
      }
      if (result) {
        count = 0;
        if (x > 0 && TILE_AT(x - 1, y) == TERRAIN_BRIDGE) {
          count++;
        }
        if (x < STATE->unk_1c4->mapWidth - 1 &&
            TILE_AT(x + 1, y) == TERRAIN_BRIDGE) {
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
        if (y > 0 && TILE_AT(x, y - 1) == TERRAIN_BRIDGE) {
          count++;
        }
        if (y < STATE->unk_1c4->mapHeight - 1 &&
            TILE_AT(x, y + 1) == TERRAIN_BRIDGE) {
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
