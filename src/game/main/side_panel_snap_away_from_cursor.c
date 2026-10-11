#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void SidePanel_SnapRight(void);
void func_020cfa14(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts the side panel on the side away from the cursor when the cursor is in
 * the top 80 pixels of the screen; lower down, the panel keeps its side. */
void SidePanel_SnapAwayFromCursor(void)
{
    Unk_02080440_48 *state = STATE;
    s16 screenX = state->unk_28 * 16 - state->unk_1c4->scrollX;
    s16 screenY = state->unk_2a * 16 - state->unk_1c4->scrollY;

    if (screenY < 80) {
        if (screenX < 128) {
            SidePanel_SnapRight();
        } else {
            func_020cfa14();
        }
    } else if (state->sidePanelRight == 0) {
        func_020cfa14();
    } else {
        SidePanel_SnapRight();
    }
}
