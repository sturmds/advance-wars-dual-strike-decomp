#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/math.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

void Task_ResumeScript(void* task);

void Ov004_EaseMaskedValueTwice(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;
  int eased;

  switch (task->step) {
    case 0:
      if (task->tweenFrames == 0) {
        state->unk_8f8 = -120;
      } else {
        state->unk_8fc = -120;
      }
      task->frame = 0;
      task->duration = 70;
      task->step++;
    case 1:
      eased = Math_Ease(4, task->tweenFrom, task->tweenTo, task->frame,
                        task->duration);
      if (task->tweenFrames == 0) {
        state->unk_900 = eased & 0xffff;
      } else {
        state->unk_904 = eased & 0xffff;
      }
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        task->step++;
      }
      break;
    case 2:
      task->frame = 0;
      task->duration = 70;
      task->step++;
    case 3:
      eased = Math_Ease(1, task->tweenTo, task->unk_5c, task->frame,
                        task->duration);
      if (task->tweenFrames == 0) {
        state->unk_900 = eased & 0xffff;
      } else {
        state->unk_904 = eased & 0xffff;
      }
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        Task_ResumeScript(task);
      }
      break;
  }
}
