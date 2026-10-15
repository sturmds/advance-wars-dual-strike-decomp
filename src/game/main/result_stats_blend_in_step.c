#include "types.h"
#include "game/screen_other_map.h"
#include "game/win_lose.h"
#include "game/math.h"

void Screen_SetCurrentBoth(void* state);
void Task_ResumeScript(void* task);
void* func_0203b614(void); /* adds the menu button data_0215a538 */
void WinLoseScreen_DrawPicturesOneSide(WinLoseTask* task);

#define BLEND_FRAMES 64

/* Script task step of the stats screen, 64 frames: draws the CO pictures
 * (WinLoseScreen_DrawPicturesOneSide) each frame and eases the alpha blend
 * of both screens, first target from 2 to 9 and second target from 16 to 9
 * (frame 1 only sets the first to 1). Then adds a menu button and resumes
 * the script. */
void ResultStats_BlendInStep(WinLoseTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  WinLoseScreen_DrawPicturesOneSide(task);
  task->frame++;
  savedGfx = data_027c0280;
  savedState = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  if (task->frame == 1) {
    data_027c0280->blendEva = 1;
  } else {
    data_027c0280->blendEva = Math_Ease(4, 2, 9, task->frame, BLEND_FRAMES);
    data_027c0280->blendEvb = Math_Ease(4, 16, 9, task->frame, BLEND_FRAMES);
  }
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  if (task->frame == 1) {
    data_027c0280->blendEva = 1;
  } else {
    data_027c0280->blendEva = Math_Ease(4, 2, 9, task->frame, BLEND_FRAMES);
    data_027c0280->blendEvb = Math_Ease(4, 16, 9, task->frame, BLEND_FRAMES);
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (task->frame != BLEND_FRAMES) {
    return;
  }
  task->frame = 0;
  func_0203b614();
  Task_ResumeScript(task);
}
