#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"
#include "game/terrain.h"
#include "game/Unk_ov006_0235f690.h"

int Ov006_CountMatchingNeighbors(int x, int y);
s16 Ov006_GetShoalGraphic(int x, int y);
int func_ov006_023604a4(int x, int y);
int Ov006_IsBareReefOrSea(int x, int y);
int Ov006_CanPlaceBridge(int x, int y);
int Ov006_CannotPlacePipeSeam(int x, int y);
void Map_ScrollViewToTile(int arg);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define OV6 data_ov006_0236d640
#define TILE(x, y) (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

/* Checks whether the chosen tool can be used on the cursor tile (cursorX,
 * cursorY) and stores the answer in toolResult: 1 allowed, 6 not allowed, 5 when
 * flag 0x40 is set in unit mode. In terrain mode (unitMode == 0) toolTile holds
 * the terrain to paint, and shoal, river, reef, bridge, road over a river
 * and pipe seam each have their own test; any other terrain is refused only
 * while flag 0x2000 is set. In unit mode the unit in toolUnit must be able to
 * move onto the tile. Map_ScrollViewToTile gets 5 or 1; its name may be
 * wrong here (a cursor style fits better). */
int Ov006_CheckToolAtCursor(void) {
  Unk_ov006_0235f690* ov6 = OV6;
  int result;

  if (ov6->unitMode == 0) {
    int tool = ov6->toolTile;

    result = 1;
    if (tool == TERRAIN_SHOAL) {
      if (Ov006_CountMatchingNeighbors(ov6->cursorX, ov6->cursorY) < 0 ||
          Ov006_GetShoalGraphic(OV6->cursorX, OV6->cursorY) < 0) {
        result = 6;
      }
    } else if (tool == TERRAIN_RIVER) {
      if (func_ov006_023604a4(ov6->cursorX, ov6->cursorY) == 0) {
        result = 6;
      }
    } else if (tool == TERRAIN_REEF) {
      if (Ov006_IsBareReefOrSea(ov6->cursorX, ov6->cursorY) == 0) {
        result = 6;
      }
    } else if (tool == TERRAIN_BRIDGE) {
      if (Ov006_CanPlaceBridge(ov6->cursorX, ov6->cursorY) == 0) {
        result = 6;
      }
    } else if (tool == TERRAIN_ROAD) {
      if (TILE(ov6->cursorX, ov6->cursorY) == TERRAIN_RIVER &&
          Ov006_CanPlaceBridge(ov6->cursorX, ov6->cursorY) == 0) {
        result = 6;
      }
    } else if (tool == TERRAIN_PIPE_SEAM) {
      if (Ov006_CannotPlacePipeSeam(ov6->cursorX, ov6->cursorY) != 0) {
        result = 6;
      }
    } else if (ov6->unk_00 & 0x2000) {
      result = 6;
    }
  } else if (ov6->unk_00 & 0x40) {
    result = 5;
  } else if (CHART[Ov000_gUnitStats[ov6->toolUnit & 63].moveType]
                  [MAP_TILE_TERRAIN(TILE(ov6->cursorX, ov6->cursorY))] != -1) {
    result = 1;
  } else {
    result = 6;
  }
  OV6->toolResult = result;
  if (result != 5) {
    result = 1;
  }
  Map_ScrollViewToTile((s16)result);
  return result;
}
