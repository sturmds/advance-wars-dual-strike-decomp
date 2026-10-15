#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_flash.h"
#include "game/math.h"

void Task_ResumeScript(Task* task);

/* Task step: brightness eases from 0 to 16; resumes the script when done. */
void ScreenFlash_UpdateFadeIn(ScreenFlashTask* flash) {
  if (flash->fadeInFrames == 0) {
    Task_ResumeScript((Task*)flash);
    return;
  }
  flash->fadeInFrame++;
  data_027c0280->masterBrightness =
      Math_Ease(0, 0, 16, flash->fadeInFrame, flash->fadeInFrames);
  if (flash->fadeInFrame != flash->fadeInFrames) {
    return;
  }
  Task_ResumeScript((Task*)flash);
}
