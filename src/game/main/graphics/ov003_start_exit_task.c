#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const TaskScriptCmd gOv003ExitTaskScript[];

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void func_01ff9c28(void (*callback)(void));
void MainLoop_SetFrameCallback(void (*callback)(void));
void MainLoop_DefaultVBlank(void);
void MainLoop_TaskFrame(void);

/* Puts the default frame callbacks back on both screens and starts the task that picks the
 * screen to go to after the Combat mode battle (Ov003_Exit). */
void Ov003_StartExitTask(void) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedState = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  MainLoop_SetFrameCallback(MainLoop_TaskFrame);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  MainLoop_SetFrameCallback(MainLoop_TaskFrame);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  func_01ff9c28(MainLoop_DefaultVBlank);
  Task_Create(gOv003ExitTaskScript, TASK_LIST(3));
}
