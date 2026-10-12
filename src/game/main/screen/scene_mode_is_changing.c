#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;
extern u8 data_02169900[];

BOOL func_020f4a18(void);
void* Task_FindByScript(void* script);

/* TRUE while a mode change is pending or its task (data_02169900) is still
 * running on the second screen. FALSE when the mode task is not running there. */
BOOL SceneMode_IsChanging(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  BOOL running;

  if (!func_020f4a18()) {
    return FALSE;
  }
  if (data_021694c0->requestedMode != data_021694c0->mode) {
    return TRUE;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  running = Task_FindByScript(data_02169900) != NULL;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return running;
}
