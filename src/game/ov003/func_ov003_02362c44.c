#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_ov003_02364bb4[];

void func_ov003_02362c44(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Task_Create(data_ov003_02364bb4, TASK_LIST(4));
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(savedGfx);
}
