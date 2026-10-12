#include "types.h"
#include "sdk/gx.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/window_wipe.h"

void DmaTasks_StartSameSource(const void* src, volatile void* dest,
                              volatile void* dest2, int unk34, int unk36,
                              void (*callback)(void));
void DmaTask_Start(const void* src, volatile void* dest, int unk34, int unk36,
                   void (*callback)(void));
void Screen_EnableEmptyWindows(void);
void Screen_ResetWindowRects(void);
void WindowWipe_BuildEdgesIn(void);

/* Task step: starts a wipe that shows the chosen screens again. Window 0
 * shows no layers and starts over the whole screen; with the progress at
 * 352, WindowWipe_BuildEdgesIn narrows it line by line, top and bottom rows
 * first, while an H-blank DMA task feeds WIN0H. */
void WindowWipe_StartShow(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (ScreenEffect_BeginShow(task) == 0) {
    return;
  }
  task->unk_64 = 1;
  if (ScreenEffect_GetScreens() == EFFECT_SCREEN_BOTH ||
      ScreenEffect_GetScreens() == EFFECT_SCREEN_MAIN ||
      (data_027c027c == data_027c0288.unk_00 &&
       ScreenEffect_GetScreens() == EFFECT_SCREEN_CURRENT)) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Screen_EnableEmptyWindows();
    Screen_ResetWindowRects();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  if (ScreenEffect_GetScreens() == EFFECT_SCREEN_BOTH ||
      ScreenEffect_GetScreens() == EFFECT_SCREEN_SUB ||
      (data_027c027c == data_027c0288.unk_04 &&
       ScreenEffect_GetScreens() == EFFECT_SCREEN_CURRENT)) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Screen_EnableEmptyWindows();
    Screen_ResetWindowRects();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  data_0216e29c = 352;
  data_0216e2a8 = 0;
  switch (ScreenEffect_GetScreens()) {
    case EFFECT_SCREEN_BOTH:
      DmaTasks_StartSameSource(gWipeWin0hTable, REG_WIN0H, REG_DB_WIN0H, 1,
                               0xa240, WindowWipe_BuildEdgesIn);
      break;
    case EFFECT_SCREEN_MAIN:
      DmaTask_Start(gWipeWin0hTable, REG_WIN0H, 1, 0xa240,
                    WindowWipe_BuildEdgesIn);
      break;
    case EFFECT_SCREEN_SUB:
      DmaTask_Start(gWipeWin0hTable, REG_DB_WIN0H, 1, 0xa240,
                    WindowWipe_BuildEdgesIn);
      break;
  }
}
