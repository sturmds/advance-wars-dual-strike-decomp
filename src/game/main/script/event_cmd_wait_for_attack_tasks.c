#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];

BOOL Attack_TasksRunning(void);

/* Event script command: waits (returns FALSE without advancing) while
 * Attack_TasksRunning reports a running task; it checks the two scripts that
 * ActingUnit_StartAttack starts. */
BOOL EventCmd_WaitForAttackTasks(s16 index) {
  if (!Attack_TasksRunning()) {
    data_022977f4[index].unk_00++;
  }
  return FALSE;
}
