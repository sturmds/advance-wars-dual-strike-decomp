#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_gen.h"

void Ov006_SetMapCell(int x, int y, int cell);
int Ov006_TryPlaceBridge(int x, int y);
int func_ov006_0235b868(int x, int y);

/* Sets road (cell 97) on every tile marked 2 in Ov006_gGenMarks, except where
 * func_ov006_0235b868 or Ov006_TryPlaceBridge returns 1. */
void Ov006_PlaceRoadsAtMarks(void) {
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (Ov006_gGenMarks[y][x] == 2 && func_ov006_0235b868(x, y) != 1 &&
          Ov006_TryPlaceBridge(x, y) != 1) {
        Ov006_SetMapCell(x, y, 97);
      }
    }
  }
}
