#include "types.h"
#include "game/keypad.h"
#include "game/ov003_unit.h"

void Ov003_ScrollWithDpad(void) {
  if (gKeypad->held & PAD_KEY_LEFT) {
    data_ov003_02364928->scrollX--;
  }
  if (gKeypad->held & PAD_KEY_RIGHT) {
    data_ov003_02364928->scrollX++;
  }
  if (gKeypad->held & PAD_KEY_UP) {
    data_ov003_02364928->scrollY--;
  }
  if (gKeypad->held & PAD_KEY_DOWN) {
    data_ov003_02364928->scrollY++;
  }
}
