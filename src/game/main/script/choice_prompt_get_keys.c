#include "types.h"
#include "game/keypad.h"

/* Maps the keys pressed this frame to the choice prompt's input bits:
 * left 4, right or B 8, A 0x10; 0 for none. The caller ORs in the touch
 * bits from ChoicePrompt_PollTouch. */
u32 ChoicePrompt_GetKeys(void) {
  u16 keys = gKeypad->nextPressed;

  if (keys & PAD_KEY_LEFT) {
    return 4;
  }
  if (keys & (PAD_KEY_RIGHT | PAD_BUTTON_B)) {
    return 8;
  }
  if (keys & PAD_BUTTON_A) {
    return 0x10;
  }
  return 0;
}
