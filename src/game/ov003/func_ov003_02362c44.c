#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
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
