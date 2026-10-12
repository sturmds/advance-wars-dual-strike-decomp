#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];

/* Returns the first live unit, other than entry skipUnit and with unk_17_6
 * clear, whose position is within 6 of (x, y); all in 8-pixel units. Returns
 * NULL when there is none. */
Ov003Unit* Ov003_FindUnitNear(int x, int y, int skipUnit) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  for (i = 0; i < 16; i++, unit++) {
    if (unit->hp != 0 && !unit->unk_17_6 && i != skipUnit) {
      int unitX = unit->x >> 3;

      if (x >= unitX - 6 && x <= unitX + 6) {
        int unitY = unit->y >> 3;

        if (y >= unitY - 6 && y <= unitY + 6) {
          return unit;
        }
      }
    }
  }
  return NULL;
}
