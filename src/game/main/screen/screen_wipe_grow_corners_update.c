#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_wipe.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);
void Screen_EnableEmptyWindows(void);
void Screen_ResetWindowRects(void);
void ScreenWipe_SetTopLeftBottomRightWindows(int width, int height);

#define WIPE_END 352

/* Task update: adds the task's speed (unk_64) to the wipe progress and eases
 * two corner windows from nothing up to 256x192 on the wipe's screens (speeds
 * up to 8 use curve 0, faster ones curve 4). At the end it empties the window
 * insides, resets the window rectangles and resumes the task's script. */
void ScreenWipe_GrowCornersUpdate(Task* task) {
  int width;
  int height;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  /* The original reads the speed signed (ldrsh). */
  data_0216e2a8 += task->unk_64;
  if (data_0216e2a8 >= WIPE_END) {
    data_0216e2a8 = WIPE_END;
  }
  if (task->unk_64 <= 8) {
    width = Math_Ease(0, 0, 256, data_0216e2a8, WIPE_END);
    height = Math_Ease(0, 0, 192, data_0216e2a8, WIPE_END);
  } else {
    width = Math_Ease(4, 0, 256, data_0216e2a8, WIPE_END);
    height = Math_Ease(4, 0, 192, data_0216e2a8, WIPE_END);
  }
  /* A window edge is 8 bits wide: 256 would wrap to 0. */
  if (width == 256) {
    width = 255;
  }

  if (ScreenEffect_GetScreens() == WIPE_BOTH ||
      ScreenEffect_GetScreens() == WIPE_SCREEN0 ||
      (data_027c027c == data_027c0288.unk_00 &&
       ScreenEffect_GetScreens() == WIPE_CURRENT)) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    ScreenWipe_SetTopLeftBottomRightWindows(width, height);
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
    ScreenWipe_SetTopLeftBottomRightWindows(width, height);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  if (data_0216e2a8 >= WIPE_END) {
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
    Task_ResumeScript(task);
  } else if (data_0216e29c >= WIPE_END) {
    /* Clamps data_0216e29c, not the progress: the original does this. */
    data_0216e29c = WIPE_END;
  }
}
