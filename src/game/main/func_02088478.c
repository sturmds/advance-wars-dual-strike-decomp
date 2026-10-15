#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/combat_setup_task.h"

/* Start of the Combat setup: each row gets its own CO index (unk_19 = row number). Unless
 * the task's unk_4e is set, the settings (map button 0, funds 20000, ...) go back to defaults. */
void func_02088478(CombatSetupTask* task) {
  Unk_02160ac0_Row* row = LINK_ROWS;
  int i;

  data_02160ac0->unk_7c = 1;
  for (i = 1; i <= COMBAT_ROW_COUNT; i++) {
    row->unk_19 = i - 1;
    row++;
  }
  if (task->unk_4e != 0) {
    return;
  }
  data_02160ac0->unk_7e = 0;
  data_02160ac0->unk_7f = 1;
  data_02160ac0->unk_78 = 20000;
  data_02160ac0->unk_80 = 1;
  data_02160ac0->unk_81 = 2;
  data_02160ac0->unk_83 = 0;
}
