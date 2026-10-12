#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"
#include "game/oam_cells.h"

extern Unk_027c0280* data_027c0280;
extern s16 gScreenShakeY;
extern s16 gScreenShakeX;

/* Puts the cells of a sprite at (x, y) into the OAM buffer, moved by the
 * screen shake offsets. x and y also carry the attribute 1 and 0 flag bits;
 * the first tile is tile, and attr2 gives the priority and palette and is
 * added to each cell's attribute 2. */
void Oam_PutCells(int x, int y, const OamCell* cell, int attr2, int tile) {
  s16 count = *(const s16*)cell;
  u16* dst;

  /* the list starts with an s16 count */
  cell = (const OamCell*)((const s16*)cell + 1);
  dst = data_027c0280->oamBuffer->unk_600;

  if (count == 0) {
    return;
  }
  do {
    dst[0] = ((cell->attr0 | y) & 0xff00) |
             ((gScreenShakeY + (cell->attr0 + y)) & 0xff);
    dst[1] = ((cell->attr1 | x) & 0xfe00) |
             ((gScreenShakeX + (cell->attr1 + x)) & 0x1ff);
    dst[2] = ((cell->attr2 & 0xfc00) + (attr2 & 0xfc00)) |
             (0x3ff & ((tile + ((cell->attr2 & 0x3ff) + (attr2 & 0x3ff))) >>
                       data_027c0280->unk_dd));
    cell++;
    dst += 4;
    data_027c0280->oamBuffer->unk_600 =
        (u8*)data_027c0280->oamBuffer->unk_600 + 8;
  } while (--count);
}
