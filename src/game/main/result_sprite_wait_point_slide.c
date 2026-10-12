#include "types.h"
#include "game/result_sprite_task.h"

void Task_ResumeScript(ResultSpriteTask* task);
void func_0203822c(int frames);

#define SLIDE_FRAMES 16

/* Script step: on its first frame starts the result point task's slide
 * (func_0203822c), then waits SLIDE_FRAMES frames and resumes the script. */
void ResultSprite_WaitPointSlide(ResultSpriteTask* task) {
  if (task->frame == 0) {
    func_0203822c(SLIDE_FRAMES);
  }
  task->frame++;
  if (task->frame == SLIDE_FRAMES) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
