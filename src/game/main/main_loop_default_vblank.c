#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

int func_01ff9f0c(void);
void func_01ff9f1c(int value);
void GfxQueue_Flush(void);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Task_RunList(Task* task);
void CallbackSlots_RunAll(void);
void OamBuffer_FlushHead(void);
void OamBuffer_FlushTail(void);
void Screen_ApplyAll(void);
void TransferQueue_Run(void);
void Gfx_CopyRequestedLayers(void);
void CallbackList_RunAndClear(void);
int Link_IsActive(void);
void func_0207d930(void);

/* Runs `body` with `screen` as the current screen, then restores the
 * previous one. */
#define ON_SCREEN(screen, body)  \
  savedGfx = data_027c0280;      \
  Screen_SetCurrentGfx(screen);  \
  savedState = data_027c027c;    \
  Screen_SetCurrent(screen);     \
  body;                          \
  Screen_SetCurrent(savedState); \
  Screen_SetCurrentGfx(savedGfx)

#define ON_BOTH_SCREENS(body)            \
  ON_SCREEN(data_027c0288.unk_00, body); \
  ON_SCREEN(data_027c0288.unk_04, body)

/* Default for the gMainLoop callback that func_01ff9c28 sets (main and
 * Map_ResetFrameCallbacks install it; probably run at VBlank). Runs task
 * list 0 on both screens every time. When the main loop step has finished a
 * frame (func_01ff9f0c, set at the end of MainLoop_RunFrame), it clears that flag
 * and sends the frame to the hardware: OAM, screen registers, palettes. */
void MainLoop_DefaultVBlank(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  if (func_01ff9f0c()) {
    GfxQueue_Flush();
  }
  ON_BOTH_SCREENS(Task_RunList(data_027c027c->taskLists[0]));
  CallbackSlots_RunAll();
  ON_BOTH_SCREENS(OamBuffer_FlushHead());
  if (func_01ff9f0c()) {
    func_01ff9f1c(0);
    ON_BOTH_SCREENS(OamBuffer_FlushTail());
    Screen_ApplyAll();
    TransferQueue_Run();
    ON_BOTH_SCREENS(Gfx_CopyRequestedLayers());
    CallbackList_RunAndClear();
  } else {
    func_01ff9f1c(0);
    if (Link_IsActive() == 1) {
      func_0207d930();
    }
  }
}
