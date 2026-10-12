#include "types.h"
#include "game/keypad.h"
#include "game/splash_task.h"
#include "game/touch.h"

void Task_ResumeScript(void* task);

/* Counts frames. After 140, A or a touch resumes the script; after 260 it
 * resumes by itself. */
void Splash_WaitUpdate(SplashTask* task) {
  BOOL skip = FALSE;
  BOOL touched; /* needed to match: separate flag */

  task->waitFrames++;
  if (task->waitFrames <= 140) {
    return;
  }
  if (task->waitFrames > 260) {
    skip = TRUE;
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    skip = TRUE;
  }
  if (gTouch.active != 0 && gTouch.phase == TOUCH_PRESSED) {
    touched = TRUE;
  } else {
    touched = FALSE;
  }
  if (touched) {
    skip = TRUE;
  }
  if (skip == TRUE) {
    Task_ResumeScript(task);
  }
}
