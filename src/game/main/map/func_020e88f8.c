#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

BOOL Map_IsCoSelectPhase(void);
Unk_02023724* Task_FindByScript(const void* script);

extern const TaskScriptCmd
    data_02168b94[]; /* task script of (opcode, argument) pairs */

/* On the first of the two game states (data_027c0288), while coSelectPhase is 1,
 * starts the data_02168b94 task on top-level list 3 unless it is already
 * running. */
void func_020e88f8(void) {
  if (data_027c027c != data_027c0288.unk_00) {
    return;
  }
  if (!Map_IsCoSelectPhase()) {
    return;
  }
  if (Task_FindByScript(data_02168b94) != NULL) {
    return;
  }
  Task_Create(data_02168b94, TASK_LIST(3));
}
