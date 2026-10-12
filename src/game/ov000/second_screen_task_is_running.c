#include "types.h"
#include "game/screen_other_map.h"

extern u8 data_ov000_023500f8[];

void* Task_FindByScript(const void* script);

int SecondScreenTask_IsRunning(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  int found;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  found = Task_FindByScript(data_ov000_023500f8) != NULL;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return found;
}
