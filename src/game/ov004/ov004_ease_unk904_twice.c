#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void* task);

void Ov004_EaseUnk904Twice(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  switch (task->step) {
    case 0: /* first ease: tweenFrom to tweenTo */
      task->frame = 0;
      task->duration = task->tweenFrames;
      task->step++;
    case 1:
      state->unk_904 = Math_Ease(0, task->tweenFrom, task->tweenTo, task->frame,
                                 task->duration);
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        task->step++;
      }
      break;
    case 2: /* second ease: tweenTo to unk_5c */
      task->frame = 0;
      task->duration = task->unk_48;
      task->step++;
    case 3:
      state->unk_904 = Math_Ease(0, task->tweenTo, task->unk_5c, task->frame,
                                 task->duration);
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        Task_ResumeScript(task);
      }
      break;
  }
}
