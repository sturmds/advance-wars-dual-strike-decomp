#include "types.h"
#include "game/Unk_02080440.h"

void Ov006_RefitTileCell(int x, int y);

/* Calls Ov006_RefitTileCell on every tile of the map, row by row. */
void func_ov006_0235cc6c(void) {
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      Ov006_RefitTileCell(x, y);
    }
  }
}
