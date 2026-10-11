#include "types.h"
#include "game/keypad.h"

/* Maps the keys pressed this frame to the event choice prompt's input bits:
 * left 4, right 8, A 0x10, B 0x20. Several bits can be set at once. */
u16 EventChoice_ReadKeys(void)
{
    u16 keys = gKeypad->nextPressed;
    u16 bits = 0;

    if (keys & PAD_KEY_LEFT) {
        bits |= 4;
    }
    if (keys & PAD_KEY_RIGHT) {
        bits |= 8;
    }
    if (keys & PAD_BUTTON_A) {
        bits |= 0x10;
    }
    if (keys & PAD_BUTTON_B) {
        bits |= 0x20;
    }
    return bits;
}
