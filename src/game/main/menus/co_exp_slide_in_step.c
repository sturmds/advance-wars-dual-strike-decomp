#include "types.h"
#include "game/co_exp.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/win_lose.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void* task);
void CoExp_DrawScreen(CoExp* exp, int a, int b, int c);

extern MenuState gMenuState;

#define SLIDE_FRAMES 16

/* Script task step, 16 frames: the reverse of CoExp_SlideStep. The offset
 * eases from 256 to 0, then the script resumes. */
void CoExp_SlideInStep(WinLoseTask* task) {
  int offset;

  task->frame++;
  offset = Math_Ease(4, 256, 0, task->frame, SLIDE_FRAMES);
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = -offset;
  CoExp_DrawScreen(
      (CoExp*)((u8*)&gMenuState + (u32) & ((MenuState*)0)->coExp.exp), offset,
      0, -1);
  if (task->frame != SLIDE_FRAMES) {
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
