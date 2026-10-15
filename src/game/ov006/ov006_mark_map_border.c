#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_gen.h"

/* Fills Ov006_gGenMarks (30 bytes per row) with 1 on the map's border
 * tiles and 0 elsewhere. */
void Ov006_MarkMapBorder(void) {
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      Ov006_gGenMarks[y][x] = 0;
      if (x == 0) {
        Ov006_gGenMarks[y][x] = 1;
      }
      if (y == 0) {
        Ov006_gGenMarks[y][x] = 1;
      }
      if (x == MAP_STATE->unk_1c4->mapWidth - 1) {
        Ov006_gGenMarks[y][x] = 1;
      }
      if (y == MAP_STATE->unk_1c4->mapHeight - 1) {
        Ov006_gGenMarks[y][x] = 1;
      }
    }
  }
}
