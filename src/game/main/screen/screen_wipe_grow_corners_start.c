#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_wipe.h"
#include "game/screen.h"

/* Task step before ScreenWipe_GrowCornersUpdate: on the wipe's screens,
 * turns on both windows with nothing inside and shrinks them to nothing,
 * then resets the wipe progress. */
void ScreenWipe_GrowCornersStart(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (ScreenEffect_BeginHide(task) == 0) {
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
    data_027c0280->win0Left = 0;
    data_027c0280->win0Top = 0;
    data_027c0280->win0Right = 0;
    data_027c0280->win0Bottom = 0;
    data_027c0280->win1Left = 0;
    data_027c0280->win1Top = 0;
    data_027c0280->win1Right = 0;
    data_027c0280->win1Bottom = 0;
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
    data_027c0280->win0Left = 0;
    data_027c0280->win0Top = 0;
    data_027c0280->win0Right = 0;
    data_027c0280->win0Bottom = 0;
    data_027c0280->win1Left = 0;
    data_027c0280->win1Top = 0;
    data_027c0280->win1Right = 0;
    data_027c0280->win1Bottom = 0;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  data_0216e29c = 0;
  data_0216e2a8 = 0;
}
