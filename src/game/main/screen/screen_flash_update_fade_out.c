#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_flash.h"

extern Unk_027c0280* data_027c0280;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* Task step: brightness eases from 16 to 0; resumes the script when done. */
void ScreenFlash_UpdateFadeOut(ScreenFlashTask* flash) {
  if (flash->fadeOutFrames == 0) {
    Task_ResumeScript((Task*)flash);
    return;
  }
  flash->fadeOutFrame++;
  data_027c0280->masterBrightness =
      Math_Ease(0, 16, 0, flash->fadeOutFrame, flash->fadeOutFrames);
  if (flash->fadeOutFrame != flash->fadeOutFrames) {
    return;
  }
  Task_ResumeScript((Task*)flash);
}
