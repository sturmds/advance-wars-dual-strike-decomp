#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

BOOL func_020f4a18(void);
int SceneMode_Request(int value);

/* On the second screen, passes field 0x10 of the data_021694c0 record to
 * SceneMode_Request, if the data_02169a10 task is running there. */
void func_020f4768(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (!func_020f4a18()) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  SceneMode_Request(data_021694c0->unk_10);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
