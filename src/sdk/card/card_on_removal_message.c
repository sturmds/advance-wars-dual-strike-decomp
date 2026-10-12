#include "sdk/card.h"

void Card_HaltAfterRemoval(void);

/* PXI receive callback for the card removal tag: runs the game's handler, then shuts down. */
void Card_OnRemovalMessage(u32 tag, u32 data, int err) {
  if ((data & 0x3f) == GAMECARD_MSG_REMOVED) {
    BOOL shutDown = TRUE;
    BOOL (*handler)(void) = data_022ac324;

    data_022ac320 = TRUE;
    if (handler) {
      shutDown = handler();
    }
    if (shutDown) {
      Card_HaltAfterRemoval();
    }
  } else {
    OS_Terminate();
  }
}
