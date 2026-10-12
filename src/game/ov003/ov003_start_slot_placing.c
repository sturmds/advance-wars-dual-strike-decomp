#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

void Ov003_DefeatTeam(int row);

/* Puts the slot's cursor on the first free cell (state 1) of its row, in
 * unit-placing mode (state = 2) with a 900 frame timer. With no free cell,
 * a linked game whose row holds a unit (state 2) switches the slot to scroll
 * mode (3); otherwise Ov003_DefeatTeam runs for the row. */
void Ov003_StartSlotPlacing(int slot) {
  int row;
  Ov003Cell* cell;
  int hasUnit;
  int i;

  hasUnit = 0;
  row = data_ov003_02364928->slots[slot].row;
  cell = data_ov003_02364928->rows[row - 1].cells;
  for (i = 0; i < 20; i++, cell++) {
    if (cell->state == 1) {
      data_ov003_02364928->slots[slot].cursor = i;
      data_ov003_02364928->slots[slot].state = 2;
      OV003_SLOT_TIMER(slot) = 900;
      return;
    }
    if (cell->state == 2) {
      hasUnit = 1;
    }
  }
  if (data_02160ac0->unk_7c != 0 && hasUnit == 1) {
    data_ov003_02364928->slots[slot].state = 3;
    return;
  }
  Ov003_DefeatTeam(row);
}
