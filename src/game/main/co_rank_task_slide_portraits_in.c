#include "types.h"
#include "game/co_rank_task.h"
#include "game/math.h"
#include "game/win_quote.h"

void Task_ResumeScript(CoRankTask* task);

/* One frame of a 24-frame slide: the portraits of CoRankTask_DrawPortraits
 * move in from 256 pixels off. Resumes the task's script on the last frame. */
void CoRankTask_SlidePortraitsIn(CoRankTask* task) {
  int offset;

  task->frame++;
  offset = Math_Ease(4, 256, 0, task->frame, 24);
  Sprite_QueueCoPortrait((offset + 64) & 0x1ff, 160, 0x6800, 0x400, 10, FALSE);
  if (task->hasBackCo == 1) {
    Sprite_QueueCoPortrait((192 - offset) & 0x1ff, 160, 0x7800, 0x5b0, 11,
                           TRUE);
  }
  if (task->frame == 24) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
