#include "types.h"
#include "game/keypad.h"

/* 0x20 (the cancel bit, as Keypad_ReadConfirmCancel returns) unless B is held. */
u32 func_020ef1e8(void)
{
    if (gKeypad->held & PAD_BUTTON_B) {
        return 0;
    }
    return 0x20;
}
