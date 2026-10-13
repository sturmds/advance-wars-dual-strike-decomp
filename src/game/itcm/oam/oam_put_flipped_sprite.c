#include "types.h"
#include "game/screen_shake.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"
#include "game/oam_cells.h"
#include "game/obj_size.h"

extern Unk_027c0280* data_027c0280;

/* Oam_PutFlippedCells without the separate tile argument: attr2 holds the
 * first tile, priority and palette, added to each cell's attribute 2. With
 * the horizontal flip bit set in x, each cell is mirrored around x and the
 * screen shake offsets are not applied. SpriteList_Draw sends only flipped
 * sprites here (Oam_PutSprite takes the rest). */
void Oam_PutFlippedSprite(int x, int y, const OamCell* cell, int attr2) {
  s16 count = *(const s16*)cell;
  u16* dst;
  u16 attr1;
  u16 attr0;
  int attr2Sum;

  /* the list starts with an s16 count */
  cell = (const OamCell*)((const s16*)cell + 1);
  dst = data_027c0280->oamBuffer->unk_600;

  if (count == 0) {
    return;
  }
  do {
    if (x & OAM_ATTR1_HFLIP) {
      s16 negWidth = -gObjSizes[cell->attr0 >> 14][cell->attr1 >> 14].width;
      s16 cellX = cell->attr1 & 0x1ff;

      if (cell->attr1 & 0x100) { /* sign-extend the 9-bit x */
        cellX |= ~0xff;
      }
      cellX = -cellX;
      attr1 = ((cell->attr1 | x) & 0xfe00) | ((negWidth + (x + cellX)) & 0x1ff);
      attr0 = ((cell->attr0 | y) & 0xff00) | ((y + cell->attr0) & 0xff);
    } else {
      attr0 = ((cell->attr0 | y) & 0xff00) |
              ((gScreenShakeY + (cell->attr0 + y)) & 0xff);
      attr1 = ((cell->attr1 | x) & 0xfe00) |
              ((gScreenShakeX + (cell->attr1 + x)) & 0x1ff);
    }
    dst[0] = attr0;
    /* needed to match: the u16 cast */
    dst[1] =
        ((cell->attr1 ^ x) & OAM_ATTR1_FLIPS) | (u16)(attr1 & ~OAM_ATTR1_FLIPS);
    attr2Sum = cell->attr2 + attr2;
    dst[2] =
        (attr2Sum & 0xfc00) | ((attr2Sum & 0x3ff) >> data_027c0280->unk_dd);
    cell++;
    dst += 4;
    data_027c0280->oamBuffer->unk_600 =
        (u8*)data_027c0280->oamBuffer->unk_600 + 8;
  } while (--count);
}
