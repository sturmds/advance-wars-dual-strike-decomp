#include "types.h"
#include "game/Unk_02080440.h"

int func_ov006_0235b46c(
    int x, int y); /* -1 when the sea tile stays sea, else a neighbour count */
BOOL Ov006_RollCellChance(int count, int cell);
void Ov006_SetMapCell(int x, int y, int cell);

#define CELL_SHOAL 0
#define CELL_SEA 42

/* Random map step: sea tiles on the coast turn into shoal, by chance
 * (Ov006_RollCellChance with the count from func_ov006_0235b46c); then a
 * second pass turns each sea tile left with a count of 2 into shoal. */
void Ov006_AddShoals(void) {
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->mapTiles->mapCells[MAP_STATE->mapTiles->rowStart[y] + x] ==
          CELL_SEA) {
        int count = func_ov006_0235b46c(x, y);
        if (count != -1 && Ov006_RollCellChance(count, CELL_SHOAL)) {
          Ov006_SetMapCell(x, y, CELL_SHOAL);
        }
      }
    }
  }
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->mapTiles->mapCells[MAP_STATE->mapTiles->rowStart[y] + x] ==
              CELL_SEA &&
          func_ov006_0235b46c(x, y) == 2) {
        Ov006_SetMapCell(x, y, CELL_SHOAL);
      }
    }
  }
}
