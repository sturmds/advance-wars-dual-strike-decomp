#include "types.h"
#include "game/Unk_02160ac0.h"

/* Resets the Combat mode setup in data_02160ac0 to its defaults: every cell
 * of the four rows empty, no cells in use, 20000 funds, and the overlay 3
 * overrides cleared. */
void CombatSetup_ResetDefaults(void) {
  Unk_02160ac0_Row* rows;
  int row;
  int i;

  rows = LINK_ROWS;
  for (row = 0; row < 4; row++, rows++) {
    for (i = 0; i < 20; i++) {
      rows->cells[i].tile = 0;
      rows->cells[i].state = 1;
    }
    rows->spawnCount = 0;
    rows->unk_19 = 1;
  }
  data_02160ac0->unk_7c = 0;
  data_02160ac0->unk_7d = 0;
  data_02160ac0->unk_7f = 0xff;
  data_02160ac0->unk_81 = 0xff;
  data_02160ac0->unk_82 = 0xff;
  data_02160ac0->unk_80 = 1;
  data_02160ac0->funds = 20000;
  data_02160ac0->co = 0;
}
