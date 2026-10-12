#include "types.h"
#include "game/keypad.h"

void LinkMenu_PushBack(void);

/* Queues the back event when B is pressed. */
void LinkMenu_CheckBackKey(void) {
  if (gKeypad->pressed & PAD_BUTTON_B) {
    LinkMenu_PushBack();
  }
}
