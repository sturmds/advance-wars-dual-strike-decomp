#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_flash.h"

extern Unk_027c0280* data_027c0280;

/* Task start: clears the frame counters; the screen starts white when there is no fade in. */
void ScreenFlash_Init(ScreenFlashTask* flash) {
  flash->fadeInFrame = 0;
  flash->holdFrame = 0;
  flash->fadeOutFrame = 0;
  if (flash->fadeInFrames == 0) {
    data_027c0280->masterBrightness = 16;
  } else {
    data_027c0280->masterBrightness = 0;
  }
}
