#include "types.h"
#include "game/Unk_02080440.h"

void Ov006_FitWaterCell(int x, int y);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->unk_1c4)

/* Runs Ov006_FitWaterCell on each tile of the 3x3 block around x, y that is
 * on the map, row by row, x, y included. */
void Ov006_FitWaterCellsInBlock(int x, int y) {
  if (y > 0) {
    if (x > 0) {
      Ov006_FitWaterCell(x - 1, y - 1);
    }
    Ov006_FitWaterCell(x, y - 1);
    if (x < MAP->mapWidth - 1) {
      Ov006_FitWaterCell(x + 1, y - 1);
    }
  }
  if (x > 0) {
    Ov006_FitWaterCell(x - 1, y);
  }
  Ov006_FitWaterCell(x, y);
  if (x < MAP->mapWidth - 1) {
    Ov006_FitWaterCell(x + 1, y);
  }
  if (y < MAP->mapHeight - 1) {
    if (x > 0) {
      Ov006_FitWaterCell(x - 1, y + 1);
    }
    Ov006_FitWaterCell(x, y + 1);
    if (x < MAP->mapWidth - 1) {
      Ov006_FitWaterCell(x + 1, y + 1);
    }
  }
}
