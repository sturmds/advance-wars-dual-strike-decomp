#include "types.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/brightness_fade.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void Task_ResumeScript(Ov004Task* task);

void Ov004Task_FadeHoldFade(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  switch ((u32)task->step) {
    case 0:
      state->unk_810 = func_020fcdc8(task, 30);
      task->step++;
    case 1:
      if (func_020fd624(state->unk_810)) {
        return;
      }
      func_020fd618(state->unk_810);
      state->unk_810 = NULL;
      task->frame = 0;
      task->duration = 60;
      task->step = 10;
    case 10:
      if (task->frame < task->duration) {
        task->frame++;
        return;
      }
      task->step = 20;
    case 20:
      state->unk_810 = BrightnessFade_StartOutBlackScreen0(task, 30);
      task->step++;
    case 21:
      if (func_020fd624(state->unk_810)) {
        return;
      }
      func_020fd618(state->unk_810);
      state->unk_810 = NULL;
      Task_ResumeScript(task);
      break;
  }
}
