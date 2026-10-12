#include "types.h"
#include "game/task.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_023559dc.h"
#include "game/brightness_fade.h"

BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void* EventScript_Start(const void* script);
void Task_ResumeScript(Ov005Task* task);
int func_ov005_02357830(void);
void func_ov005_0235785c(void);

extern const TaskScriptCmd data_ov005_02368fe0[];

/* Per-frame update: fade in (64 frames), wait 32 frames, run an event script
 * until func_ov005_02357830 reports it is done, fade out, then resume the
 * task script. */
void Ov005_UpdateEventScriptScreen(Ov005Task* task) {
  Unk_ov005_023559dc* work = task->work;

  switch (task->step) {
    case 0:
      work->fadeTask = func_020fcdc8(task, 64);
      task->step++;
    case 1:
      if (func_020fd624(work->fadeTask)) {
        return;
      }
      func_020fd618(work->fadeTask);
      task->step = 2;
      return;
    case 2:
      task->frame = 0;
      task->frames = 32;
      task->step++;
    case 3:
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step = 10;
      }
      return;
    case 10:
      EventScript_Start(data_ov005_02368fe0);
      func_ov005_0235785c();
      task->step++;
      return;
    case 11:
      if (func_ov005_02357830() == 0) {
        task->step = 100;
      }
      return;
    case 100:
      work->fadeTask = BrightnessFade_StartOutBlackScreen0(task, 64);
      task->step++;
      return;
    case 101:
      if (func_020fd624(work->fadeTask)) {
        return;
      }
      func_020fd618(work->fadeTask);
      task->step++;
      return;
    case 102:
      Task_ResumeScript(task);
      return;
  }
}
