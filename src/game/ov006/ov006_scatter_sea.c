#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_gen.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

s32 Rng_Range(s32 min, s32 max);
int Ov006_FindRandomCell(int* x, int* y, int cell);
void Ov006_SetMapCell(int x, int y, int cell);
void Ov006_GrowCells(int cell, int fromCell);
void Ov006_SettleSeedTiles(void);
void Ov006_MarkMapBorder(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_PLAIN 1
#define CELL_SEA 42
/* Ov006_gGenMarks mark for a tile that a grow step starts from. */
#define MARK_SEED 2

/* Random map step: puts 2 to 8 plus-shaped patches of sea on plain tiles
 * and grows them out over the plain (Ov006_GrowCells). */
void Ov006_ScatterSea(void) {
  int x;
  int y;
  int i;
  int count;

  Ov006_MarkMapBorder();
  count = Rng_Range(2, 8);
  for (i = 0; i < count; i++) {
    if (Ov006_FindRandomCell(&x, &y, CELL_PLAIN) == 0) {
      break;
    }
    Ov006_SetMapCell(x, y, CELL_SEA);
    Ov006_gGenMarks[y][x] = MARK_SEED;
    if (x - 1 >= 0) {
      Ov006_SetMapCell(x - 1, y, CELL_SEA);
      Ov006_gGenMarks[y][x - 1] = MARK_SEED;
    }
    if (y - 1 >= 0) {
      Ov006_SetMapCell(x, y - 1, CELL_SEA);
      Ov006_gGenMarks[y - 1][x] = MARK_SEED;
    }
    if (x + 1 < STATE->unk_1c4->mapWidth) {
      Ov006_SetMapCell(x + 1, y, CELL_SEA);
      Ov006_gGenMarks[y][x + 1] = MARK_SEED;
    }
    if (y + 1 < STATE->unk_1c4->mapHeight) {
      Ov006_SetMapCell(x, y + 1, CELL_SEA);
      Ov006_gGenMarks[y + 1][x] = MARK_SEED;
    }
  }
  Ov006_SettleSeedTiles();
  Ov006_GrowCells(CELL_SEA, 1);
}
