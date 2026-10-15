#include "types.h"
#include "game/ov003_unit.h"

int Ov003_IsPhase0OrAbove6(void);
void Ov003Unit_Draw(Ov003Unit* unit);

#define STATE data_ov003_02364928
#define MAP data_ov003_02364a88

/* Calls Ov003Unit_Draw for each unit that is in use, unless one of these
 * holds: unk_17_6 is set; the state is in mode 1 and the unit's row is neither
 * the current slot's row nor Ov003State.row; the unit stands on terrain 4 whose
 * mark is 0; safeTimer is set while unk_1c8 has bit 1 and Ov003_IsPhase0OrAbove6
 * returns 1. */
void Ov003_UpdateUnits(void) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  for (i = 0; i < OV003_UNIT_COUNT; i++, unit++) {
    int tile;

    if (unit->hp == 0) {
      continue;
    }
    if (unit->unk_17_6) {
      continue;
    }
    if (STATE->phase == 1 &&
        unit->row != STATE->slots[STATE->currentSlot].row &&
        unit->row != STATE->row) {
      continue;
    }
    tile = (unit->y >> 7) * MAP->width + (unit->x >> 7);
    if ((data_ov003_02365d10[MAP->cells[tile]] & 31) == 4 &&
        MAP->marks[tile] == 0) {
      continue;
    }
    if (unit->safeTimer != 0 && (STATE->frameCount & 2) &&
        Ov003_IsPhase0OrAbove6() == 1) {
      continue;
    }
    Ov003Unit_Draw(unit);
  }
}
