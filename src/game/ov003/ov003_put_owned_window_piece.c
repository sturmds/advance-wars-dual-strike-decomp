#include "types.h"
#include "game/ov003_unit.h"

void Oam_PutCells(int attr1, int attr0, const void* cells, int attr2, int tile);

extern u8 data_027c0030; /* cell list of a 64 by 64 sprite */

#define MARK_OWNED 1 /* Ov003Map.marks bit, see Ov003_MarkRowTiles */

#define MAP data_ov003_02364a88
#define STATE data_ov003_02364928

/* Puts piece tile, flipped by flip (OAM attribute 1 bits 12-13), as an OBJ
 * window sprite (attribute 0 mode 2) 16 pixels up and left of tile x, y on
 * screen. Needed to match: the scroll fields are read as signed. */
#define PUT(flip, tile)                                             \
  Oam_PutCells((((x << 4) - STATE->scrollX - 16) & 0x1ff) | (flip), \
               (((y << 4) - STATE->scrollY - 16) & 0xff) | 0x800,   \
               &data_027c0030, 0, (tile))

/* Shapes the OBJ window over the four tiles from x, y to x + 1, y + 1: picks
 * a 64 by 64 piece and its flip from which of the four are MARK_OWNED.
 * Bits of corners: 1 x, y; 2 x + 1, y; 4 x, y + 1; 8 x + 1, y + 1. Pieces:
 * 0 one tile, 64 two side by side, 128 two above each other, 192 two on a
 * diagonal, 256 three, 320 all four. */
void Ov003_PutOwnedWindowPiece(int x, int y) {
  int corners = 0;
  int width = MAP->width;
  int lastX;
  int lastY;

  if (MAP->marks[x + y * width] & MARK_OWNED) {
    corners |= 1;
  }
  lastX = width - 1;
  if (x < lastX && (MAP->marks[x + 1 + y * width] & MARK_OWNED)) {
    corners |= 2;
  }
  lastY = MAP->height - 1;
  if (y < lastY && (MAP->marks[x + (y + 1) * width] & MARK_OWNED)) {
    corners |= 4;
  }
  if (x < lastX && y < lastY &&
      (MAP->marks[x + 1 + (y + 1) * width] & MARK_OWNED)) {
    corners |= 8;
  }
  switch (corners) {
    case 1:
      PUT(0, 0);
      break;
    case 2:
      PUT(0x1000, 0);
      break;
    case 4:
      PUT(0x2000, 0);
      break;
    case 8:
      PUT(0x3000, 0);
      break;
    case 3:
      PUT(0, 64);
      break;
    case 12:
      PUT(0x2000, 64);
      break;
    case 5:
      PUT(0, 128);
      break;
    case 10:
      PUT(0x1000, 128);
      break;
    case 9:
      PUT(0, 192);
      break;
    case 6:
      PUT(0x1000, 192);
      break;
    case 7:
      PUT(0, 256);
      break;
    case 11:
      PUT(0x1000, 256);
      break;
    case 13:
      PUT(0x2000, 256);
      break;
    case 14:
      PUT(0x3000, 256);
      break;
    case 15:
      PUT(0, 320);
      break;
  }
}
