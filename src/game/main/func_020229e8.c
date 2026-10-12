#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void func_0200640c(int a0, Task* parent);

BOOL func_020229e8(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  func_0200640c(task->pc->arg, task);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_0200640c(task->pc->arg, task);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  task->pc++;
  return FALSE;
}
