#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_wipe.h"

void Screen_EnableEmptyWindows(void);
void Screen_ResetWindowRects(void);

/* Task step before ScreenWipe_ShrinkCornersUpdate: on the wipe's screens,
 * turns on both windows with nothing inside and sets window 0 to cover the
 * whole screen, then starts the wipe progress at 0. */
void ScreenWipe_ShrinkCornersStart(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (ScreenEffect_BeginShow(task) == 0) {
    return;
  }
  task->unk_64 = 1;
  if (ScreenEffect_GetScreens() == WIPE_BOTH ||
      ScreenEffect_GetScreens() == WIPE_SCREEN0 ||
      (data_027c027c == data_027c0288.unk_00 &&
       ScreenEffect_GetScreens() == WIPE_CURRENT)) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Screen_EnableEmptyWindows();
    Screen_ResetWindowRects();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  if (ScreenEffect_GetScreens() == WIPE_BOTH ||
      ScreenEffect_GetScreens() == WIPE_SCREEN1 ||
      (data_027c027c == data_027c0288.unk_04 &&
       ScreenEffect_GetScreens() == WIPE_CURRENT)) {
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
}
