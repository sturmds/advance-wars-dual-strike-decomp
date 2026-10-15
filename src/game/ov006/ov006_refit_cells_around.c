#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_GetWaterCell(int x, int y);
void Ov006_StoreMapCell(int x, int y, int cell);
void Ov006_RefitTileByTerrain(int x, int y);
void Ov006_RefitJoinedCellsInCross(int x, int y);

#define REFIT(nx, ny)                                     \
  Ov006_StoreMapCell(nx, ny, Ov006_GetWaterCell(nx, ny)); \
  Ov006_RefitTileByTerrain(nx, ny)

/* Picks the cells of the eight tiles around x, y again (those on the map):
 * first the cell Ov006_GetWaterCell gives, then Ov006_RefitTileByTerrain.
 * The tile x, y itself is left alone; Ov006_RefitJoinedCellsInCross runs last. */
void Ov006_RefitCellsAround(int x, int y) {
  if (y > 0) {
    if (x > 0) {
      REFIT(x - 1, y - 1);
    }
    REFIT(x, y - 1);
    if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
      REFIT(x + 1, y - 1);
    }
  }
  if (x > 0) {
    REFIT(x - 1, y);
  }
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    REFIT(x + 1, y);
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      REFIT(x - 1, y + 1);
    }
    REFIT(x, y + 1);
    if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
      REFIT(x + 1, y + 1);
    }
  }
  Ov006_RefitJoinedCellsInCross(x, y);
}
