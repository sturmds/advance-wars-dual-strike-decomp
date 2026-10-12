#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];

/* Refills the hp of every unit of the row (Ov003Unit.row holds the row number). */
void Ov003_HealRowUnits(int row) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  for (i = 0; i < 16; i++, unit++) {
    if (unit->hp != 0 && unit->row == row) {
      unit->hp = 100;
    }
  }
}
