#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0235a30c.h"
#include "game/Unk_ov005_0235b574.h"

extern Unk_027c0280 *data_027c0280;

int Ov005_CharToGlyphIndex(int code);
void func_ov005_0235b4c0(void *work, int x, int y);
void TransferQueue_Add(const void *src, void *dst, u32 size);

/* 72%: the original keeps three induction variables for the outer loop and
 * loads the tile into a register that lives past the glyph loop. Draws the text of each entry of the first
 * table into the second buffer and queues its transfer to object VRAM. */
void func_ov005_0235b3d4(Unk_ov005_0235a30c *work)
{
    int i = 0;

    for (;;) {
        int j;

        if (work->entriesA[i].id == 0) {
            return;
        }
        for (j = 0; j < 32; j++) {
            int glyph = 63;

            if (j < work->entriesA[i].length) {
                glyph = Ov005_CharToGlyphIndex(work->entriesA[i].text[j]);
            }
            if (work->entriesA[i].style == 2) {
                glyph += 64;
            } else if (work->entriesA[i].style == 3) {
                glyph += 64;
            }
            func_ov005_0235b4c0(work, j + (work->entriesA[i].tile >> 1), glyph);
        }
        TransferQueue_Add(&((Unk_ov005_0235b574 *)work)->buffer_2010[work->entriesA[i].tile << 5],
                          data_027c0280->objVram + (work->entriesA[i].tile << 5), 0x800);
        i++;
    }
}
