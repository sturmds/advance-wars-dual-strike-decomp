#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define SLIDE_FRAMES 64

/* Task update: over 64 frames, scrolls the sub screen's layer 2 from 48
 * pixels down to its place (BG y scroll -48 to 0) while its alpha blend
 * goes from EVA 0 / EVB 16 to EVA 16 / EVB 0. On the last frame it turns
 * the blend off and resumes the task's script. That reset runs after the
 * previous screen is current again, so it does not touch the sub screen
 * unless that was the current one. */
void SubScreenLayer2_SlideInUpdate(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int bg;

  FRAME(task)++;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  bg = data_027c0280->layer2Bg;
  data_027c0280->bgScrollY[bg] =
      Math_Ease(4, -48, 0, FRAME(task), SLIDE_FRAMES);
  data_027c0280->blendEva = Math_Ease(0, 0, 16, FRAME(task), SLIDE_FRAMES);
  data_027c0280->blendEvb = 16 - data_027c0280->blendEva;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  if (FRAME(task) == SLIDE_FRAMES) {
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    Task_ResumeScript(task);
  }
}
