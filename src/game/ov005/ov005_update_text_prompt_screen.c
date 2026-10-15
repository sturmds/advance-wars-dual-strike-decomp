#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb80.h"
#include "game/screen.h"

void func_02004d3c(int a);
void func_02004d64(int a);
Task* func_020fcb08(int a);
BOOL BrightnessFade_IsActive(Task* task);
void func_020fccb0(Task* task);
BOOL Text_IsPrinting(void);
void Text_StopPrinting(void);
void Task_Destroy(void* task);
void Task_ResumeScript(Ov005Task* task);
void Ov005Ripple_ReleaseObjects(void);
void Ov005_UpdateAffineBgScreen(void);
void func_ov005_023531d8(void);

static TaskScriptCmd data_ov005_02368020[] = {
#include "data/ov005/data_ov005_02368020.inc.c"
};
extern MenuButtonTemplate data_ov005_02368070;

/* Opens a text box with a child task, waits for the text to finish, then
 * for A or button 253 (a touch). Closes the box, fades the song out and
 * resumes the task script. */
void Ov005_UpdateTextPromptScreen(Ov005Task* task) {
  Unk_ov005_0236bb80* work = task->work;

  switch (task->step) {
    case 0: {
      Task* child = Task_Create(data_ov005_02368020, task);
      ((Ov005Task*)child)->work = work;
      work->textTask = child;
      func_02004d3c(64);
      task->step++;
    }
    case 1:
      if (!Screens_IsTransitionRunning()) {
        task->step = 10;
      }
      return;
    case 10:
      if (!Text_IsPrinting()) {
        task->step++;
      }
      return;
    case 11:
      MenuButton_Add(0, 0, &data_ov005_02368070, 253);
      MenuButton_Select(253);
      work->unk_08 = 0;
      task->step++;
    case 12:
      if (gKeypad->pressed & PAD_BUTTON_A) {
        task->step = 100;
        return;
      }
      if (work->unk_08 != 0) {
        task->step = 100;
      }
      return;
    case 100:
      func_02004d64(64);
      task->step++;
      return;
    case 101:
      if (!Screens_IsTransitionRunning()) {
        task->step++;
      }
      return;
    case 102:
      work->songTask = func_020fcb08(1);
      task->step++;
    case 103:
      if (BrightnessFade_IsActive(work->songTask)) {
        return;
      }
      func_020fccb0(work->songTask);
      MenuButton_Remove(253);
      Task_Destroy(work->textTask);
      Text_StopPrinting();
      Task_ResumeScript(task);
      return;
  }
}
