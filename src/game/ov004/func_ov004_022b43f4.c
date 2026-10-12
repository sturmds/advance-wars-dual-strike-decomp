#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void* task);

void func_ov004_022b43f4(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->duration = 57;
      task->step++;
    case 1:
      state->unk_908 = Math_Ease(4, task->tweenFrom, task->tweenTo, task->frame,
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
