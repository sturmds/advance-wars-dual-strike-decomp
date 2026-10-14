#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern TaskScriptCmd data_02169a10[];

BOOL func_020f4a18(void);
void func_020f37d0(void);
void Task_DestroyByScript(void* script);

/* Stops the data_02169a10 task on the second screen, if it is running there. */
void func_020f4870(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (!func_020f4a18()) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  func_020f37d0();
  Task_DestroyByScript(data_02169a10);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
