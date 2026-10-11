#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0235a30c.h"
#include "game/Unk_ov005_0235b574.h"

extern Unk_027c0280 *data_027c0280;

int Ov005_CharToGlyphIndex(int code);
void func_ov005_0235b4c0(void *work, int x, int y);
void TransferQueue_Add(const void *src, void *dst, u32 size);

/* Needed to match: the original walks the table with three counters (a moving pointer, a byte
 * offset and an unused index) and rebuilds the entry pointer from the offset on each pass. */
#pragma opt_strength_reduction off
#pragma opt_loop_invariants off
#pragma opt_dead_assignments off

/* `work` moved forward by `off` bytes: its entriesA[0] is entry off / sizeof(entry) of the table. */
#define WORK_AT(off) ((Unk_ov005_0235a30c *)((u8 *)work + (off)))

/* Draws the text of each entry of the first table into the tile buffer, 32 glyphs per entry with
 * glyph 63 past the text, then queues the entry's 0x800 bytes of tiles to object VRAM. Styles 2
 * and 3 use the second half of the font (glyph + 64). */
void Ov005_DrawCreditsEntryText(Unk_ov005_0235a30c *work)
{
    int j;
    int glyph;
    int tile;
    int i;
    Unk_ov005_0235a30c *entry;
    Unk_ov005_0235a30c *row;
    int off;

    for (i = 0, row = work, off = 0;; i++) {
        if (row->entriesA[0].id == 0) {
            return;
        }
        entry = WORK_AT(off);
        for (j = 0; j < 32; j++) {
            glyph = 63;
            tile = entry->entriesA[0].tile;
            if (j < entry->entriesA[0].length) {
                glyph = Ov005_CharToGlyphIndex(entry->entriesA[0].text[j]);
            }
            if (entry->entriesA[0].style == 2) {
                glyph += 64;
            } else if (entry->entriesA[0].style == 3) {
                glyph += 64;
            }
            func_ov005_0235b4c0(work, j + (tile >> 1), glyph);
        }
        TransferQueue_Add(&((Unk_ov005_0235b574 *)work)->buffer_2010[tile << 5],
                          data_027c0280->objVram + (tile << 5), 0x800);
        row = (Unk_ov005_0235a30c *)((u8 *)row + sizeof(Unk_ov005_0235abfc));
        off += sizeof(Unk_ov005_0235abfc);
    }
}
