#include "types.h"
#include "game/keypad.h"

/* Folds this frame's held keys into keypad: opposite D-pad directions
 * cancel, then newly pressed keys and auto-repeat are worked out.
 * unk_08 is the keys held now, unk_0e the keys held last frame, unk_0a the
 * pressed or repeating keys and unk_0c the newly pressed ones; the public
 * fields at the top are copies of these. */
void Keypad_UpdateState(Keypad *keypad, u16 keys)
{
    if ((keys & (PAD_KEY_RIGHT | PAD_KEY_LEFT)) == (PAD_KEY_RIGHT | PAD_KEY_LEFT)) {
        keys ^= PAD_KEY_RIGHT | PAD_KEY_LEFT;
    }
    if ((keys & (PAD_KEY_UP | PAD_KEY_DOWN)) == (PAD_KEY_UP | PAD_KEY_DOWN)) {
        keys ^= PAD_KEY_UP | PAD_KEY_DOWN;
    }
    keypad->unk_0e = keypad->unk_08;
    keypad->unk_08 = keys;
    keypad->unk_0a = keypad->unk_08 & (keypad->unk_08 ^ keypad->unk_0e);
    keypad->unk_0c = keypad->unk_0a;
    if (keypad->unk_08 != 0 && keypad->unk_08 == keypad->unk_0e) {
        keypad->repeatCountdown--;
        if (keypad->repeatCountdown == 0) {
            keypad->unk_0a = keypad->unk_08;
            keypad->repeatCountdown = keypad->repeatInterval;
        }
    } else {
        keypad->repeatCountdown = keypad->repeatDelay;
    }
    keypad->unk_00 = keypad->unk_08;
    keypad->pressed = keypad->unk_0c;
    keypad->repeat = keypad->unk_0a;
    keypad->unk_06 = keypad->unk_0e;
}
