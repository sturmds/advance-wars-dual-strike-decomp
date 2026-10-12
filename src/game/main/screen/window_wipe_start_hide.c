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
void WindowWipe_BuildCenterOut(void);

/* Task step: starts a wipe that hides the chosen screens behind window 0.
 * Window 0 shows no layers; WindowWipe_BuildCenterOut widens it line by
 * line from the middle rows outwards while an H-blank DMA task feeds WIN0H. */
void WindowWipe_StartHide(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (ScreenEffect_BeginHide(task) == 0) {
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
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  data_0216e29c = 0;
  data_0216e2a8 = 0;
  switch (ScreenEffect_GetScreens()) {
    case EFFECT_SCREEN_BOTH:
      DmaTasks_StartSameSource(gWipeWin0hTable, REG_WIN0H, REG_DB_WIN0H, 1,
                               0xa240, WindowWipe_BuildCenterOut);
      break;
    case EFFECT_SCREEN_MAIN:
      DmaTask_Start(gWipeWin0hTable, REG_WIN0H, 1, 0xa240,
                    WindowWipe_BuildCenterOut);
      break;
    case EFFECT_SCREEN_SUB:
      DmaTask_Start(gWipeWin0hTable, REG_DB_WIN0H, 1, 0xa240,
                    WindowWipe_BuildCenterOut);
      break;
  }
}
