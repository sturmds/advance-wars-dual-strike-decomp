#include "types.h"
#include "game/zoom_sprite_task.h"

void Task_ResumeScript(void* task);

/* Like ZoomSpriteTask_Update, but once the scale reaches 1x the sprite stays
 * for 60 frames and then the task's script resumes. */
void ZoomSpriteTask_UpdateThenResume(ZoomSpriteTask* task) {
  int next;

  ZoomSpriteTask_Draw(task);
  if (task->holdFrames > 0) {
    task->holdFrames--;
    if (task->holdFrames == 0) {
      Task_ResumeScript(task);
    }
  } else {
    next = task->scale - (task->scale - 0x100) / 2;
    if (next - 0x100 == 4) {
      task->holdFrames = 60;
      next = 0x100;
    }
    task->scale = next;
  }
}
