#include "types.h"
#include "game/screen_shake.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"
#include "game/oam_cells.h"

extern Unk_027c0280* data_027c0280;

/* Puts the cells of a sprite at (x, y) into the OAM buffer like
 * Oam_PutCells, but takes the attribute 0 and 1 flag bits apart from the
 * position and skips cells whose top-left corner is off screen (x outside
 * -63..255, y outside -63..191). */
void Oam_PutClippedCells(int x, int y, int attr1Flags, int attr0Flags,
                         const OamCell* cell, int attr2, int tile) {
  s16 count = *(const s16*)cell;
  u16* dst;

  /* the list starts with an s16 count */
  cell = (const OamCell*)((const s16*)cell + 1);
  dst = data_027c0280->oamBuffer->unk_600;

  if (count == 0) {
    return;
  }
  do {
    int cellY = y + (s8)cell->attr0;

    if (cellY > -64 && cellY < 192) {
      s16 cellX = cell->attr1 & 0x1ff;

      if (cellX & 0x100) {
        cellX |= 0xff00;
      }
      if (cellX + x > -64 && cellX + x < 256) {
        dst[0] = ((cell->attr0 | attr0Flags) & 0xff00) |
                 ((gScreenShakeY + (cell->attr0 + y)) & 0xff);
        dst[1] = ((cell->attr1 | attr1Flags) & 0xfe00) |
                 ((gScreenShakeX + (cell->attr1 + x)) & 0x1ff);
        dst[2] = ((cell->attr2 & 0xfc00) + (attr2 & 0xfc00)) |
                 (0x3ff & ((tile + ((cell->attr2 & 0x3ff) + (attr2 & 0x3ff))) >>
                           data_027c0280->unk_dd));
        dst += 4;
        data_027c0280->oamBuffer->unk_600 =
            (u8*)data_027c0280->oamBuffer->unk_600 + 8;
      }
    }
    cell++;
  } while (--count);
}
