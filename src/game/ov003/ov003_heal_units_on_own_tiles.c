#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];
extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 row, bits 0-4 terrain */

int Ov003_IsTerrainSpecial(int terrain); /* guess: terrain lets a unit heal */

/* A unit that stands on a tile of its own row (and Ov003_IsTerrainSpecial accepts
 * the terrain) counts frames in unk_17_0. At 36 it gains 1 hp and the count
 * restarts at 30, so it gains 1 hp every 6 frames after that. Any other tile
 * resets the count. */
void Ov003_HealUnitsOnOwnTiles(void) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  for (i = 0; i < 16; i++, unit++) {
    u8 tile;

    if (unit->hp == 0) {
      continue;
    }
    if (unit->hp == 100) {
      unit->unk_17_0 = 0;
      continue;
    }
    tile =
        data_ov003_02365d10[data_ov003_02364a88->cells
                                [(unit->y >> 7) * data_ov003_02364a88->width +
                                 (unit->x >> 7)]];
    if (Ov003_IsTerrainSpecial(tile) == 0) {
      unit->unk_17_0 = 0;
      continue;
    }
    if (((tile >> 5) & 7) != unit->row) {
      unit->unk_17_0 = 0;
      continue;
    }
    unit->unk_17_0++;
    if (unit->unk_17_0 == 36) {
      unit->hp++;
      unit->unk_17_0 = 30;
    }
  }
}
