#include "types.h"
#include "game/task.h"
#include "game/screen_layer0_blend.h"
#include "game/ex_machina_bolt_task.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

#define FADE_FRAMES 5

/* Script step of gExMachinaBoltScript, after ExMachinaBolt_StartFlash: sets
 * alpha blending and eases blendEva from 0 to 16 over FADE_FRAMES frames,
 * then resumes the script. */
void ExMachinaBolt_FadeIn(ExMachinaBoltTask* task) {
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = Math_Ease(0, 0, 16, task->frame, FADE_FRAMES);
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  task->frame++;
  if (task->frame > FADE_FRAMES) {
    Task_ResumeScript((Task*)task);
  }
}
