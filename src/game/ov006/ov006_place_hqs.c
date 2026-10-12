#include "types.h"
#include "game/tile_point.h"

s32 Rng_Range(s32 min, s32 max);
BOOL Ov006_PickRandomCellBlock(int* outX, int* outY, int cell,
                               TilePoint* avoid);
TilePoint Ov006_PlaceHq(int x, int y, int owner, int baseCount,
                        int airportCount);
TilePoint Ov006_PlaceRandomHq(int owner, int baseCount, int airportCount);

#define CELL_PLAIN 1

/* Random map step: puts the HQs of armies 1 and 2 on 3x3 blocks of plain,
 * at least 15 tiles apart when such blocks are found (else anywhere through
 * Ov006_PlaceRandomHq). Both armies get the same 2-4 bases and 1-3
 * airports around their HQ. */
void Ov006_PlaceHqs(TilePoint* hq1, TilePoint* hq2) {
  int baseCount = Rng_Range(2, 4);
  int airportCount = Rng_Range(1, 3);
  int x;
  int y;

  if (Ov006_PickRandomCellBlock(&x, &y, CELL_PLAIN, NULL) == TRUE) {
    *hq1 = Ov006_PlaceHq(x, y, 1, baseCount, airportCount);
  } else {
    *hq1 = Ov006_PlaceRandomHq(1, baseCount, airportCount);
  }
  if (Ov006_PickRandomCellBlock(&x, &y, CELL_PLAIN, hq1) == TRUE) {
    *hq2 = Ov006_PlaceHq(x, y, 2, baseCount, airportCount);
  } else {
    *hq2 = Ov006_PlaceRandomHq(2, baseCount, airportCount);
  }
}
