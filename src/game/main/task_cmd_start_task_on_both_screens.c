#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void func_02006398(int a0);

/* Task script command: starts a top-level task (func_02006398) with the command's
 * argument on both screens, then moves on to the next command. */
BOOL TaskCmd_StartTaskOnBothScreens(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_02006398(task->pc->arg);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_02006398(task->pc->arg);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  task->pc++;
  return TRUE;
}
