#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

int func_ov004_022bb014(Unk_ov004_022bf158* state, int value);
void Task_ResumeScript(void* task);

void func_ov004_022b6834(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  switch (task->step) {
    case 0:
      task->tweenFrames = 0;
      task->frame = 0;
      task->duration = 140;
      task->step++;
    case 1:
      func_ov004_022bb014(state, (u16)task->tweenFrames);
      task->tweenFrames -= task->tweenFrom;
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        Task_ResumeScript(task);
      }
      break;
  }
}
