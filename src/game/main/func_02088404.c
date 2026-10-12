#include "types.h"
#include "game/Unk_02160ac0.h"

extern Unk_02160ac0* data_02160ac0;

/* Resets the four rows of data_02160ac0: every cell gets tile 0 and state 1, the spawn count is
 * cleared and the funds are set to the value in unk_78. */
void func_02088404(void) {
  Unk_02160ac0_Row* row = LINK_ROWS;
  int i;
  int j;

  for (i = 1; i <= 4; i++) {
    for (j = 0; j < 20; j++) {
      row->cells[j].tile = 0;
      row->cells[j].state = 1;
    }
    row->spawnCount = 0;
    row->funds = data_02160ac0->unk_78;
    row++;
  }
}
