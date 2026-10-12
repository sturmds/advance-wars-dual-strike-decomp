#include "types.h"
#include "game/screen_flash.h"

void Task_ResumeScript(Task* task);

/* Task step: waits holdFrames frames, then resumes the script. */
void ScreenFlash_UpdateHold(ScreenFlashTask* flash) {
  if (flash->holdFrames == 0) {
    Task_ResumeScript((Task*)flash);
    return;
  }
  flash->holdFrame++;
  if (flash->holdFrame != flash->holdFrames) {
    return;
  }
  Task_ResumeScript((Task*)flash);
}
