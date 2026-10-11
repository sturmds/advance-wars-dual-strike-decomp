#include "types.h"
#include "game/keypad.h"

/* 0x20 for a B press, 0x10 for an A press, else 0. */
u32 Keypad_ReadConfirmCancel(void)
{
    u16 keys = gKeypad->pressed;

    if (keys & PAD_BUTTON_B) {
        return 0x20;
    }
    if (keys & PAD_BUTTON_A) {
        return 0x10;
    }
    return 0;
}
