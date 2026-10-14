#include "types.h"
#include "game/task.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_023559dc.h"
#include "game/brightness_fade.h"

BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
Task* Sound_StartSongSwitchTask(int a, int b, int c);
Task* func_020fcb08(int a);
BOOL BrightnessFade_IsActive(Task* task);
void func_020fccb0(Task* task);
void* EventScript_Start(const void* script);
void Task_ResumeScript(Ov005Task* task);
int func_ov005_02357830(void);
void func_ov005_0235785c(void);
void func_ov005_02357844(void);

TaskScriptCmd data_ov005_02368e40[52] = {
#include "data/ov005/data_ov005_02368e40.inc.c"
};
static TaskScriptCmd data_ov005_02368ca0[52] = {
#include "data/ov005/data_ov005_02368ca0.inc.c"
};
TaskScriptCmd data_ov005_02368fe0[52] = {
#include "data/ov005/data_ov005_02368fe0.inc.c"
};

/* Like Ov005_UpdateEventScriptScreen, but starts a sound task (Sound_StartSongSwitchTask
 * with id 51) before the script and fades the song out (func_020fcb08)
 * after the closing fade, before the task script resumes. */
void Ov005_UpdateEventScriptSongScreen(Ov005Task* task) {
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
      work->songFadeTask = Sound_StartSongSwitchTask(51, 0, 8);
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
      EventScript_Start(data_ov005_02368ca0);
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
      work->songFadeTask = func_020fcb08(1);
      task->step++;
    case 103:
      if (BrightnessFade_IsActive(work->songFadeTask)) {
        return;
      }
      func_020fccb0(work->songFadeTask);
      Task_ResumeScript(task);
      return;
  }
}
