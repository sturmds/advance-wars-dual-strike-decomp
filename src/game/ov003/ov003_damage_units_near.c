#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];

u32 Math_Distance(int x1, int y1, int x2, int y2);
int func_ov003_0235d468(int row);

/* Hits the units of other rows within 24 pixels of (x, y): 30 hp off (down to 1),
 * then safeTimer = 15 so they are not hit again for 15 frames. Units of a row
 * whose func_ov003_0235d468 is set are skipped. */
void Ov003_DamageUnitsNear(int row, int x, int y) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  for (i = 0; i < 16; i++, unit++) {
    /* needed to match: the (u16) cast makes the load unsigned; the field is s16 */
    if (unit->hp == 0 || unit->unk_17_6 || unit->row == row ||
        unit->safeTimer != 0) {
      continue;
    }
    if (func_ov003_0235d468(unit->row) != 0) {
      continue;
    }
    if (Math_Distance(x, y, unit->x >> 3, unit->y >> 3) >= 24) {
      continue;
    }
    if (unit->hp > 30) {
      unit->hp -= 30;
    } else {
      unit->hp = 1;
    }
    unit->safeTimer = 15;
  }
}
