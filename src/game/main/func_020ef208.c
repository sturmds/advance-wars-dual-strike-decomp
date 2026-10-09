#include "types.h"
#include "game/keypad.h"

u32 func_020ef208(void)
{
    u16 keys = gKeypad->repeat;

    if (keys & (PAD_KEY_RIGHT | PAD_KEY_DOWN)) {
        return 8;
    }
    if (keys & (PAD_KEY_LEFT | PAD_KEY_UP)) {
        return 4;
    }
    return 0;
}
