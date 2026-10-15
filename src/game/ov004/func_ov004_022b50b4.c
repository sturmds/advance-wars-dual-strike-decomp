#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/math.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

void Task_ResumeScript(void* task);

void func_ov004_022b50b4(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->duration = task->tweenFrames;
      task->step++;
    case 1:
      state->unk_8f8 = Math_Ease(0, task->tweenFrom, task->tweenTo, task->frame,
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
