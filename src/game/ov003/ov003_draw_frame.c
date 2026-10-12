#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

#define REG_SWAP_BUFFERS (*(volatile u32*)0x04000540)

void Ov003_Begin3d(void);
void Ov003_Update(void);
void EventScript_UpdateAll(void);
void ScriptSlot_RunAll(void);
void Ov003_End3d(void);
void Task_RunList(Task* task);

/* Per-frame callback installed by Ov003_Init: sets up the 3D scene, runs the
 * task lists 1, 2, 3, 5 and 4 in that order, then swaps the 3D buffers. */
void Ov003_DrawFrame(void) {
  Ov003_Begin3d();
  Ov003_Update();
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  EventScript_UpdateAll();
  ScriptSlot_RunAll();
  Task_RunList(data_027c027c->taskLists[1]);
  Task_RunList(data_027c027c->taskLists[2]);
  Task_RunList(data_027c027c->taskLists[3]);
  Task_RunList(data_027c027c->taskLists[5]);
  Task_RunList(data_027c027c->taskLists[4]);
  Ov003_End3d();
  REG_SWAP_BUFFERS = 1;
}
