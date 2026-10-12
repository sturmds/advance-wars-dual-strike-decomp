#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];

u32 Math_Distance(int x1, int y1, int x2, int y2);
int func_ov003_0235d468(int row);

/* Like Ov003_DamageUnitsNear, but x and y are in unit coordinates (8 per pixel)
 * and the radius is 35 pixels. */
void Ov003_DamageUnitsAround(int row, int x, int y) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;
  int pixelY = y >> 3;
  int pixelX = x >> 3;

  for (i = 0; i < 16; i++, unit++) {
    /* needed to match: the (u16) cast makes the load unsigned; the field is s16 */
    if (unit->hp == 0 || unit->unk_17_6 || unit->row == row ||
        unit->safeTimer != 0) {
      continue;
    }
    if (func_ov003_0235d468(unit->row) != 0) {
      continue;
    }
    if (Math_Distance(pixelX, pixelY, unit->x >> 3, unit->y >> 3) >= 35) {
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
