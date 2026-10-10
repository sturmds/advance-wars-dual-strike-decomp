#include "types.h"
#include "game/keypad.h"
#include "game/ov003_unit.h"

/* Needed to match: unk_1d0 and unk_1d2 are u16 in Ov003State, but the original
 * reads them signed. */
#define SCROLL_X (*(s16 *)&data_ov003_02364928->unk_1d0)
#define SCROLL_Y (*(s16 *)&data_ov003_02364928->unk_1d2)

void Ov003_ScrollWithDpad(void)
{
    if (gKeypad->unk_00 & PAD_KEY_LEFT) {
        SCROLL_X--;
    }
    if (gKeypad->unk_00 & PAD_KEY_RIGHT) {
        SCROLL_X++;
    }
    if (gKeypad->unk_00 & PAD_KEY_UP) {
        SCROLL_Y--;
    }
    if (gKeypad->unk_00 & PAD_KEY_DOWN) {
        SCROLL_Y++;
    }
}
