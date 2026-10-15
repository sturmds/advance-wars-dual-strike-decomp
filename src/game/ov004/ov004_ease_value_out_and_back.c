#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/math.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

void Task_ResumeScript(void* task);

void Ov004_EaseValueOutAndBack(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  switch (task->step) {
    case 0: /* ease out: tweenFrom to tweenTo */
      task->frame = 0;
      task->duration = 39;
      task->step++;
    case 1:
      if (task->tweenFrames == 0) {
        state->unk_8f8 = Math_EaseOvershoot(110, task->tweenFrom, task->tweenTo,
                                            task->frame, task->duration);
      } else {
        state->unk_8fc = Math_EaseOvershoot(110, task->tweenFrom, task->tweenTo,
                                            task->frame, task->duration);
      }
      if (task->frame < task->duration) {
        task->frame++;
        return;
      }
      task->step++;
    case 2: /* ease back: unk_5c to tweenTo, played in reverse */
      task->frame = 0;
      task->duration = 39;
      task->step++;
    case 3:
      if (task->tweenFrames == 0) {
        state->unk_8f8 =
            Math_EaseOvershoot(110, task->unk_5c, task->tweenTo,
                               task->duration - task->frame, task->duration);
      } else {
        state->unk_8fc =
            Math_EaseOvershoot(110, task->unk_5c, task->tweenTo,
                               task->duration - task->frame, task->duration);
      }
      if (task->frame < task->duration) {
        task->frame++;
        return;
      }
      Task_ResumeScript(task);
      break;
  }
}
