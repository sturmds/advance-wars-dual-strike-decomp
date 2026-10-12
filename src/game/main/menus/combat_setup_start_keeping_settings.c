#include "types.h"
#include "game/task.h"
#include "game/combat_setup_task.h"

extern const TaskScriptCmd gCombatSetupTaskScript[];

/* Starts the Combat setup task; it keeps the settings already in data_02160ac0 (func_02088478). */
void CombatSetup_StartKeepingSettings(void) {
  CombatSetupTask* task =
      (CombatSetupTask*)Task_Create(gCombatSetupTaskScript, TASK_LIST(3));

  task->unk_4e = 1;
}
