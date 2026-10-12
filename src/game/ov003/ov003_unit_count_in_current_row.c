#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];

/* Counts the entries in use (hp set, or unk_17_7) whose row equals the current row. */
int Ov003Unit_CountInCurrentRow(void) {
  int count;
  Ov003Unit* unit;
  int i;
  Ov003State* state;
  count = 0;
  unit = data_ov003_02369538;
  state = data_ov003_02364928;

  for (i = 0; i < 16; i++, unit++) {
    if (unit->hp != 0 || unit->unk_17_7) {
      if (unit->row == state->row) {
        count++;
      }
    }
  }
  return count;
}
