#include "types.h"
#include "game/keypad.h"

#define REG_KEYINPUT (*(volatile u16*)0x04000130)
#define HW_BUTTON_XY_BUF \
  (*(volatile u16*)0x027fffa8) /* X, Y and debug bits, written by the ARM7 */
#define PAD_ALL_MASK 0x2fff

extern Keypad data_0216e594;

void Keypad_UpdateState(Keypad* keypad, u16 keys);

/* Reads the keys held now (KEYINPUT is active low, so the bits are inverted) and updates
 * the keypad that gKeypad points to. */
void Keypad_Poll(void) {
  Keypad_UpdateState(
      &data_0216e594,
      ((REG_KEYINPUT | HW_BUTTON_XY_BUF) ^ PAD_ALL_MASK) & PAD_ALL_MASK);
}
