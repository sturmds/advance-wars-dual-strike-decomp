#include "types.h"
#include "game/result_sprite_task.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(ResultSpriteTask* task);

#define SCROLL_FRAMES 16

/* Script step: eases the horizontal scroll of layer 1 from 0 to 88 over
 * SCROLL_FRAMES frames, then resumes the script. */
void ResultSprite_ScrollLayer(ResultSpriteTask* task) {
  u8 bg;

  task->frame++;
  bg = data_027c0280->layer1Bg;
  data_027c0280->bgScrollX[bg] =
      Math_Ease(4, 0, 88, task->frame, SCROLL_FRAMES);
  if (task->frame != SCROLL_FRAMES) {
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
