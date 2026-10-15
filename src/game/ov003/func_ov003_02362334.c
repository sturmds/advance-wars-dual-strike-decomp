#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov003_unit.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_ov003_02364bf4[];

void func_ov003_02362334(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_ov003_02364928->phases = (data_ov003_02364928->phases & ~0xf0) | 0x70;
  Task_Create(data_ov003_02364bf4, TASK_LIST(4));
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(savedGfx);
}
