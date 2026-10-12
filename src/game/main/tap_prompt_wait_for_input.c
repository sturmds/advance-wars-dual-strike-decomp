#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/touch.h"

void TapPrompt_Draw(void);
void Sound_PlaySe(u32 soundId);
void func_020763b4(void);
void Task_ResumeScript(Task* task);

/* Script step: draws the prompt sprite each frame; A, B, Start or a touch
 * plays the confirm sound, starts the task of func_020763b4 and resumes the script. */
void TapPrompt_WaitForInput(Task* task) {
  TapPrompt_Draw();
  if (!(gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START))) {
    if (gTouch.phase != TOUCH_PRESSED) {
      return;
    }
    if (gTouch.active != 1) {
      return;
    }
  }
  Sound_PlaySe(0x10006);
  func_020763b4();
  Task_ResumeScript(task);
}
