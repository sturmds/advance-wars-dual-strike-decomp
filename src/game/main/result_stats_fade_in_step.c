#include "types.h"
#include "game/screen_other_map.h"
#include "game/win_lose.h"

void Screen_SetCurrentBoth(void* state);
void Task_ResumeScript(void* task);
void WinLoseScreen_DrawPicturesOneSide(WinLoseTask* task);

#define FADE_FRAMES 32

/* Script task step, 32 frames: draws the CO pictures
 * (WinLoseScreen_DrawPicturesOneSide) each frame and lowers the brightness
 * of both screens from 16 (white) by one every two frames, then resumes the
 * script. */
void ResultStats_FadeInStep(WinLoseTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  WinLoseScreen_DrawPicturesOneSide(task);
  task->frame++;
  savedGfx = data_027c0280;
  savedState = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = 16 - (task->frame >> 1);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = 16 - (task->frame >> 1);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (task->frame != FADE_FRAMES) {
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
