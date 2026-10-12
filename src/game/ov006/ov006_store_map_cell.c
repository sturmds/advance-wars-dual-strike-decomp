#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Stores a map cell code (9 bits) at tile x, y; a negative code is ignored. */
void Ov006_StoreMapCell(int x, int y, int cell) {
  MapTiles* map;

  if (cell < 0) {
    return;
  }
  map = data_027c027c->unk_48[data_027c0284]->mapTiles;
  map->mapCells[map->rowStart[y] + x] = cell & 0x1ff;
}
