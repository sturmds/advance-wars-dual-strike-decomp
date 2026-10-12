#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/ov003_unit.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern const TaskScriptCmd data_ov003_02364acc[];
void Sound_PlaySe(u32 soundId);

/* Does nothing unless Ov003State.phase and nextPhase are 0. Otherwise plays
 * sound 243, sets nextPhase to 6 and starts the data_ov003_02364acc task
 * for the row (func_ov003_0235ca24 destroys it). */
void func_ov003_0235ca50(int rowNumber) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  Task* task;

  if (data_ov003_02364928->phase != 0) {
    return;
  }
  if (data_ov003_02364928->phase != data_ov003_02364928->nextPhase) {
    return;
  }
  Sound_PlaySe(243);
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_ov003_02364928->nextPhase = 6;
  task = Task_Create(data_ov003_02364acc, TASK_LIST(4));
  task->unk_52 = rowNumber;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
