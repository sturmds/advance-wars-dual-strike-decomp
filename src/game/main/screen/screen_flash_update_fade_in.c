#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_flash.h"

extern Unk_027c0280* data_027c0280;

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
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
