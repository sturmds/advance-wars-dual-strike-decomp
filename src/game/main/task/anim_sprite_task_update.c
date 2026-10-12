#include "types.h"
#include "game/anim_sprite_task.h"
#include "game/task.h"

void AnimSpriteTask_Draw(AnimSpriteTask* task);
void Task_ResumeScript(Task* task);

/* Task update: draws the current frame, then (unless paused) runs commands
 * until one shows a frame. A frame command shows its frame for its low
 * byte + 1 updates. */
void AnimSpriteTask_Update(AnimSpriteTask* task) {
  u16 cmd;

  if (task->frame != NULL) {
    AnimSpriteTask_Draw(task);
  }
  if (task->paused != NULL && *task->paused != 0) {
    return;
  }
  for (;;) {
    cmd = *task->cmd;
    switch (cmd & ANIM_SPRITE_CMD_MASK) {
      case ANIM_SPRITE_CMD_END:
        task->playing = 0;
        if (task->callback != NULL && !task->callbackRan) {
          task->callback(task);
        }
        Task_ResumeScript((Task*)task);
        return;
      case ANIM_SPRITE_CMD_HOLD:
        task->held = 1;
        return;
      case ANIM_SPRITE_CMD_LOOP:
        task->cmd = task->start;
        task->timer = 0;
        break;
      case ANIM_SPRITE_CMD_CALLBACK:
        task->callback(task);
        task->callbackRan = 1;
        task->cmd++;
        break;
      case ANIM_SPRITE_CMD_SET_5B:
        task->unk_5b = cmd;
        task->cmd++;
        break;
      default:
        task->frame = task->frames[cmd >> 8];
        if (task->timer == (*task->cmd & 0xff)) {
          task->cmd++;
          task->timer = 0;
        }
        task->timer++;
        return;
    }
  }
}
