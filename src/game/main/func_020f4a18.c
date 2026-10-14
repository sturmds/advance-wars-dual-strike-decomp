#include "types.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern TaskScriptCmd data_02169a10[];

void* Task_FindByScript(void* script);

/* TRUE when the data_02169a10 task is running on the second screen (data_027c0288.unk_04). */
BOOL func_020f4a18(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  BOOL found;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  found = Task_FindByScript(data_02169a10) != NULL;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return found;
}
