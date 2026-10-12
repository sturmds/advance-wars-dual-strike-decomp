#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

s32 Rng_Range(s32 min, s32 max);
void Ov006_SetMapCell(int x, int y, int cell);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[STATE->mapTiles->rowStart[y] + (x)])
#define CELL_PLAIN 1
/* Owned property cells come in fives per owner from 448 (see
 * Ov006_SetMapCell): HQ, base, city, airport, port. ownerCells is
 * owner * 5. */
#define CELL_HQ(ownerCells) (448 + (ownerCells))
#define CELL_BASE(ownerCells) (449 + (ownerCells))
#define CELL_AIRPORT(ownerCells) (451 + (ownerCells))

/* Puts owner's HQ at x, y, then baseCount bases and airportCount airports on
 * random plain tiles within 2 of it. A miss picks again without counting, so
 * it does not return while too few plain tiles are left. Returns x, y. */
TilePoint Ov006_PlaceHq(int x, int y, int owner, int baseCount,
                        int airportCount) {
  TilePoint hq;
  int ownerCells = owner * 5; /* needed to match: kept in a local */
  int i;
  int j;
  int bx;
  int by;

  Ov006_SetMapCell(x, y, CELL_HQ(ownerCells));
  for (i = 0; i < baseCount; i++) {
    bx = Rng_Range(x - 2, x + 2);
    by = Rng_Range(y - 2, y + 2);
    if (bx < 0 || bx >= STATE->unk_1c4->mapWidth || by < 0 ||
        by >= STATE->unk_1c4->mapHeight) {
      i--;
      continue;
    }
    /* needed to match: the second bounds test jumps back into this block */
    if (CELL_AT(bx, by) != CELL_PLAIN) {
    baseFail:
      i--;
      continue;
    }
    if (by < 0 || by >= STATE->unk_1c4->mapHeight || bx < 0 ||
        bx >= STATE->unk_1c4->mapWidth) {
      goto baseFail;
    }
    Ov006_SetMapCell(bx, by, CELL_BASE(ownerCells));
  }
  for (j = 0; j < airportCount; j++) {
    bx = Rng_Range(x - 2, x + 2);
    by = Rng_Range(y - 2, y + 2);
    if (CELL_AT(bx, by) != CELL_PLAIN) {
    airportFail:
      j--;
      continue;
    }
    if (by < 0 || by >= STATE->unk_1c4->mapHeight || bx < 0 ||
        bx >= STATE->unk_1c4->mapWidth) {
      goto airportFail;
    }
    Ov006_SetMapCell(bx, by, CELL_AIRPORT(ownerCells));
  }
  hq.x = x;
  hq.y = y;
  return hq;
}
