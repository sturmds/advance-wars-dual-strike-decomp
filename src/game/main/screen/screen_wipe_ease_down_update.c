#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_wipe.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);
void Screen_SetCurrentBoth(void* state);
void ScreenWipe_EaseDownSetWindow1Rows(void);

#define WIPE_END 352

/* Task update: adds the task's speed (unk_64) to the wipe progress and eases
 * data_0216e29c from 352 down to 0 (curve 0 for speeds up to 8, else curve 1).
 * Each frame ScreenWipe_EaseDownSetWindow1Rows updates window 1's rows on the wipe's screens. At
 * 0 it turns both windows off and resumes the task's script. */
void ScreenWipe_EaseDownUpdate(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  /* The original reads the speed signed (ldrsh). */
  data_0216e2a8 += task->unk_64;
  if (data_0216e2a8 >= WIPE_END) {
    data_0216e2a8 = WIPE_END;
  }
  if (task->unk_64 <= 8) {
    data_0216e29c = Math_Ease(0, WIPE_END, 0, data_0216e2a8, WIPE_END);
  } else {
    data_0216e29c = Math_Ease(1, WIPE_END, 0, data_0216e2a8, WIPE_END);
  }

  if (ScreenEffect_GetScreens() == WIPE_BOTH ||
      ScreenEffect_GetScreens() == WIPE_SCREEN0 ||
      (data_027c027c == data_027c0288.unk_00 &&
       ScreenEffect_GetScreens() == WIPE_CURRENT)) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    ScreenWipe_EaseDownSetWindow1Rows();
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
    ScreenWipe_EaseDownSetWindow1Rows();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  if (data_0216e29c > 0) {
    return;
  }
  data_0216e29c = 0;
  switch (ScreenEffect_GetScreens()) {
    case WIPE_CURRENT:
      GFX_DISPCNT(data_027c0280)->win0 = 0;
      GFX_DISPCNT(data_027c0280)->win1 = 0;
      break;
    case WIPE_BOTH:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      GFX_DISPCNT(data_027c0280)->win0 = 0;
      GFX_DISPCNT(data_027c0280)->win1 = 0;
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      GFX_DISPCNT(data_027c0280)->win0 = 0;
      GFX_DISPCNT(data_027c0280)->win1 = 0;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case WIPE_SCREEN0:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      GFX_DISPCNT(data_027c0280)->win0 = 0;
      GFX_DISPCNT(data_027c0280)->win1 = 0;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case WIPE_SCREEN1:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      GFX_DISPCNT(data_027c0280)->win0 = 0;
      GFX_DISPCNT(data_027c0280)->win1 = 0;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
  }
  Task_ResumeScript(task);
}
