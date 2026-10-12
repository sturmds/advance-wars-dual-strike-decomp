#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_edit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))
#define CELL(state, x, y) \
  ((state)->mapTiles->mapCells[(x) + (state)->mapTiles->rowStart[y]])

/* The bridge piece that tile x, y would continue: a vertical bridge below
 * or above it, else a horizontal bridge right or left of it. -1 when none
 * of them is there. */
int Ov006_GetAdjacentBridgeCell(int x, int y) {
  Unk_02080440_1c4* map;
  Unk_02080440_48* state;
  int cell;

  state = STATE;
  map = state->unk_1c4;

  if (y < map->mapHeight - 1) {
    cell = CELL(state, x, y + 1);
    if (cell == MAP_CELL_BRIDGE_VERTICAL) {
      return cell;
    }
  }
  if (y > 0) {
    cell = CELL(state, x, y - 1);
    if (cell == MAP_CELL_BRIDGE_VERTICAL) {
      return cell;
    }
  }
  if (x < map->mapWidth - 1) {
    cell = CELL(state, x + 1, y);
    if (cell == MAP_CELL_BRIDGE_HORIZONTAL) {
      return cell;
    }
  }
  if (x > 0) {
    cell = CELL(state, x - 1, y);
    if (cell == MAP_CELL_BRIDGE_HORIZONTAL) {
      return cell;
    }
  }
  return -1;
}
