#include "types.h"
#include "game/task.h"
#include "game/Unk_02168140.h"
#include "game/screen_other_map.h"

extern const TaskScriptCmd data_02168140[];
extern void* data_02167e98[];

/* Starts the data_02168140 task on this screen's map state with arguments 8, 8 and data_02167e98[0]. Name kept: the script's purpose is unknown. */
void func_ov001_02351af0(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_02168140Task* task;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
  task = (Unk_02168140Task*)Task_Create(data_02168140, TASK_LIST(3));
  task->unk_2c = 8;
  task->unk_30 = 8;
  task->unk_4c = data_02167e98[0];
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
