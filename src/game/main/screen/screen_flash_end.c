#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_flash.h"

extern Unk_027c0280* data_027c0280;

/* Task end: back to normal brightness if the flash faded out. */
void ScreenFlash_End(ScreenFlashTask* flash) {
  if (flash->fadeOutFrames != 0) {
    data_027c0280->masterBrightness = 0;
  }
}
