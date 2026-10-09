#include "types.h"
#include "game/keypad.h"

u32 func_02024a40(void)
{
    u16 keys = gKeypad->unk_0c;

    if (keys & PAD_BUTTON_START) {
        return 0x400;
    }
    if (keys & PAD_BUTTON_A) {
        return 0x10;
    }
    return 0;
}
