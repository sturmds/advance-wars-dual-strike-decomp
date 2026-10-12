#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];
extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 row, bits 0-4 terrain */

#define MAP data_ov003_02364a88
#define TERRAIN_AT(unit)                                          \
  (data_ov003_02365d10[MAP->cells[((unit)->y >> 7) * MAP->width + \
                                  ((unit)->x >> 7)]] &            \
   31)

/* For each live unit whose tile field is 1 or 2 (not the tile code its name
 * suggests): moves 1 to 2 when the unit stands on terrain 3, and 2 back to 1
 * when it does not. */
void Ov003_UpdateUnitTerrain(void) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  for (i = 0; i < 16; i++, unit++) {
    if (unit->hp != 0) {
      switch (unit->kind) {
        case 1:
          if (TERRAIN_AT(unit) == 3) {
            unit->kind = 2;
          }
          break;
        case 2:
          if (TERRAIN_AT(unit) != 3) {
            unit->kind = 1;
          }
          break;
      }
    }
  }
}
