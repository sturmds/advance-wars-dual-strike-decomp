#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/task.h"
#include "game/win_quote.h"

void WinQuote_Close(MenuButton* button);

/* Last step of the win quote screen: draws the COs until A, B or START is
 * pressed, then closes the screen through WinQuote_Close. */
void WinQuote_WaitKeys(Task* task) {
  WinQuote_DrawCos(task);
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
    WinQuote_Close(NULL);
  }
}
