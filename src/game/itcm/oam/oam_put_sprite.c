#include "types.h"
#include "game/screen_shake.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"
#include "game/oam_cells.h"

extern Unk_027c0280* data_027c0280;

/* Oam_PutCells without the separate tile argument: attr2 holds the first
 * tile, priority and palette, and is added to each cell's attribute 2.
 * Unk_027c0280.unk_dd shifts the tile number right (OBJ tile steps larger
 * than 32 bytes); the loop without the shift is kept apart. */
void Oam_PutSprite(int x, int y, const OamCell* cell, int attr2) {
  s16 count = *(const s16*)cell;
  u16* dst;

  /* the list starts with an s16 count */
  cell = (const OamCell*)((const s16*)cell + 1);
  dst = data_027c0280->oamBuffer->unk_600;

  if (data_027c0280->unk_dd == 0) {
    if (count == 0) {
      return;
    }
    do {
      dst[0] = ((cell->attr0 | y) & 0xff00) |
               ((gScreenShakeY + (cell->attr0 + y)) & 0xff);
      dst[1] = ((cell->attr1 | x) & 0xfe00) |
               ((gScreenShakeX + (cell->attr1 + x)) & 0x1ff);
      dst[2] = cell->attr2 + attr2;
      cell++;
      dst += 4;
      data_027c0280->oamBuffer->unk_600 =
          (u8*)data_027c0280->oamBuffer->unk_600 + 8;
    } while (--count);
  } else {
    if (count == 0) {
      return;
    }
    do {
      dst[0] = ((cell->attr0 | y) & 0xff00) |
               ((gScreenShakeY + (cell->attr0 + y)) & 0xff);
      dst[1] = ((cell->attr1 | x) & 0xfe00) |
               ((gScreenShakeX + (cell->attr1 + x)) & 0x1ff);
      dst[2] =
          ((cell->attr2 & 0xfc00) + (attr2 & 0xfc00)) |
          (((cell->attr2 & 0x3ff) + (attr2 & 0x3ff)) >> data_027c0280->unk_dd);
      cell++;
      dst += 4;
      data_027c0280->oamBuffer->unk_600 =
          (u8*)data_027c0280->oamBuffer->unk_600 + 8;
    } while (--count);
  }
}
