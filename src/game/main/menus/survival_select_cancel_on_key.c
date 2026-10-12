#include "types.h"
#include "game/keypad.h"

void Sound_PlaySe(u32 soundId);
void SurvivalSelect_Leave(void);

/* B leaves the Survival map select. */
void SurvivalSelect_CancelOnKey(void) {
  if (gKeypad->pressed & PAD_BUTTON_B) {
    Sound_PlaySe(0x10009);
    SurvivalSelect_Leave();
  }
}
