#include "types.h"
#include "game/keypad.h"

extern Keypad data_0216e4e4[9];   /* the last entry is data_0216e594, the one gKeypad points to */
extern Keypad data_0216e594;

void Keypad_Init(void)
{
    int i;
    Keypad *key = data_0216e4e4;

    for (i = 0; i < 9; i++, key++) {
        key->unk_06 = 0;
        key->unk_00 = 0;
        key->pressed = 0;
        key->repeatCountdown = 0;
        key->repeat = 0;
        key->unk_0e = 0;
        key->unk_08 = 0;
        key->unk_0c = 0;
        key->unk_0a = 0;
        key->repeatDelay = 12;
        key->repeatInterval = 4;
    }
    gKeypad = &data_0216e594;
}
