#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_020cfb48(void);
void func_020cfc40(void);
void func_020e16e0(int x);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Per-frame version of SidePanel_SnapAwayFromCursor: the panel slides to the
 * side away from the cursor (func_020cfc40 slides it right, func_020cfb48 left)
 * instead of jumping, then is placed at its new x. */
void SidePanel_SlideAwayFromCursor(void)
{
    Unk_02080440_48 *state = STATE;
    s16 screenX = state->unk_28 * 16 - state->unk_1c4->scrollX;
    s16 screenY = state->unk_2a * 16 - state->unk_1c4->scrollY;

    if (screenY < 80) {
        if (screenX < 128) {
            func_020cfc40();
        } else {
            func_020cfb48();
        }
    } else if (state->sidePanelRight == 1) {
        func_020cfc40();
    } else {
        func_020cfb48();
    }
    func_020e16e0(STATE->sidePanelX);
}
