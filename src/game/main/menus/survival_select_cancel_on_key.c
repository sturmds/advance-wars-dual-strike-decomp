#include "types.h"
#include "game/keypad.h"

void Sound_PlaySe(u32 soundId);
void func_02045b44(void);

/* B leaves the Survival map select. */
void SurvivalSelect_CancelOnKey(void)
{
    if (gKeypad->pressed & PAD_BUTTON_B) {
        Sound_PlaySe(0x10009);
        func_02045b44();
    }
}
