#include "types.h"
#include "game/keypad.h"

void Sound_PlaySe(u32 soundId);
void Menu_Dismiss(void);

void Menu_DismissOnPress(void) {
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
    Sound_PlaySe(0x10006);
    Menu_Dismiss();
  }
}
