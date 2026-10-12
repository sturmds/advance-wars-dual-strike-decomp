#include "types.h"
#include "game/keypad.h"

void WinLoseScreen_DrawPicturesOneSide(void);
void ResultStats_ClosePage(void);

/* Script task step on a stats page: draws the CO pictures
 * (WinLoseScreen_DrawPicturesOneSide) every frame and closes the page when
 * A, B or Start is pressed. */
void ResultStats_WaitForClose(void) {
  WinLoseScreen_DrawPicturesOneSide();
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
    ResultStats_ClosePage();
  }
}
