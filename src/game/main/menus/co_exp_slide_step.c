#include "types.h"
#include "game/co_exp.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/win_lose.h"
#include "game/math.h"

void ScriptTask_ResumeWithValue(int value);
void Task_ResumeScript(void* task);
void CoExp_DrawScreen(CoExp* exp, int a, int b, int c);

#define SLIDE_FRAMES 16

/* Script task step, 16 frames: scrolls layer 1 sideways by an eased 0 to
 * 256 and moves the CO cells by the same amount, then resumes the script.
 * On the first frame it also passes 16 to ScriptTask_ResumeWithValue. */
void CoExp_SlideStep(WinLoseTask* task) {
  int offset;

  if (task->frame == 0) {
    ScriptTask_ResumeWithValue(SLIDE_FRAMES);
  }
  task->frame++;
  offset = Math_Ease(1, 0, -256, task->frame, SLIDE_FRAMES);
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
