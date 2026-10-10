#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/Unk_ov005_0236bba8.h"

int Co_GetRank(u32 co);

extern Unk_ov005_0236bba8 *data_ov005_0236bba8;

/* work->unk_288 read as CoPaletteButton[CO_PALETTE_BUTTONS]. The shared
 * header types unk_288 as bytes; indexing a cast of it adds the field
 * offset first and does not match. */
#define PALETTE_BUTTON(work, i) \
    (*(CoPaletteButton *)((u8 *)(work) + (i) * sizeof(CoPaletteButton) + ((u32)&((Unk_ov005_0236bba8 *)0)->unk_288)))

/* Moves the cursor over the CO's palette variant buttons with the d-pad.
 * Left and right wrap around the 8 variants of row 0, and the last one
 * there is kept in unk_214. Down goes to the ninth variant (row 1), up
 * comes back; both need CO rank 10. */
void Ov005_MoveCoPaletteCursor(void)
{
    Unk_ov005_0236bba8 *work;
    int variant;
    u16 keys;
    int vertical;
    s16 col;
    s16 row;
    int horizontal;

    work = data_ov005_0236bba8;
    keys = gKeypad->repeat;

    vertical = 0;
    if (keys & PAD_KEY_UP) {
        vertical = 1;
    } else if (keys & PAD_KEY_DOWN) {
        vertical = 2;
    }
    horizontal = 0;
    if (keys & PAD_KEY_LEFT) {
        horizontal = 1;
    } else if (keys & PAD_KEY_RIGHT) {
        horizontal = 2;
    }

    if (Co_GetRank(work->coId) < 10) {
        vertical = 0;
    }
    variant = work->unk_20c;
    col = PALETTE_BUTTON(work, variant).col;
    row = PALETTE_BUTTON(work, variant).row;

    if (vertical == 0 && row == 0) {
        if (horizontal == 1) {
            if (col > 0) {
                variant--;
            } else {
                variant += 7;
            }
        } else if (horizontal == 2) {
            if (col < 7) {
                variant++;
            } else {
                variant -= 7;
            }
        }
        work->unk_214 = variant;
    } else if (vertical == 1) {
        if (row == 1) {
            variant = work->unk_214;
        }
    } else if (row == 0) {
        variant = 8;
    }

    MenuButton_Select(PALETTE_BUTTON(work, variant).buttonId);
    work->unk_20c = variant;
}
