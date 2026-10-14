#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->unk_1c4)
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

/* Cell codes of one family of joined pieces, named by the sides they join.
 * The _ALT straights pass every test the plain ones pass; they may be pipe
 * seams (a guess). 0x120-0x122 join left and right, 0x123 and 0x102-0x103
 * join up and down, but this function never accepts them as the centre. */
#define CELL_RIGHT_DOWN 0x140
#define CELL_LEFT_DOWN 0x141
#define CELL_HORIZONTAL 0x142
#define CELL_VERTICAL 0x143
#define CELL_RIGHT_UP 0x160
#define CELL_LEFT_UP 0x161
#define CELL_HORIZONTAL_ALT 0x162
#define CELL_VERTICAL_ALT 0x163

/* True when cell is one of count codes from first on. Needed to match: the
 * original subtracts through a u16 constant (cell + 0x10000 - first). */
#define CELL_IN_RANGE(cell, first, count) \
  ((u16)((cell) + (u16) - (first)) <= (count) - 1)

/* The piece at cell has an end on its right (left, bottom, top) side. */
#define JOINS_RIGHT(c)                                                         \
  ((c) == CELL_HORIZONTAL || (c) == CELL_RIGHT_DOWN || (c) == CELL_RIGHT_UP || \
   (c) == CELL_HORIZONTAL_ALT || CELL_IN_RANGE(c, 0x120, 3))
#define JOINS_LEFT(c)                                                        \
  ((c) == CELL_HORIZONTAL || (c) == CELL_LEFT_DOWN || (c) == CELL_LEFT_UP || \
   (c) == CELL_HORIZONTAL_ALT || CELL_IN_RANGE(c, 0x120, 3))
#define JOINS_DOWN(c)                                                         \
  ((c) == CELL_VERTICAL || (c) == CELL_RIGHT_DOWN || (c) == CELL_LEFT_DOWN || \
   (c) == CELL_VERTICAL_ALT || (c) == 0x123 || CELL_IN_RANGE(c, 0x102, 2))
#define JOINS_UP(c)                                                       \
  ((c) == CELL_VERTICAL || (c) == CELL_RIGHT_UP || (c) == CELL_LEFT_UP || \
   (c) == CELL_VERTICAL_ALT || (c) == 0x123 || CELL_IN_RANGE(c, 0x102, 2))

/* 1 when the cell at x, y is a straight or corner piece of the family above
 * and both of its ends meet a neighbour on the map that joins back. */
int Ov006_IsJoinedPieceConnected(int x, int y) {
  u16 cell = CELL_AT(x, y);

  if (cell == CELL_HORIZONTAL || cell == CELL_VERTICAL ||
      cell == CELL_RIGHT_DOWN || cell == CELL_LEFT_DOWN ||
      cell == CELL_RIGHT_UP || cell == CELL_LEFT_UP ||
      cell == CELL_HORIZONTAL_ALT || cell == CELL_VERTICAL_ALT ||
      cell == 0x122 || cell == 0x123 || cell == 0x121 || cell == 0x120 ||
      (u32)(cell - 0x102) <=
          1) { /* needed to match: an unsigned range test without the u16 wrap */
    if (cell == CELL_HORIZONTAL || cell == CELL_HORIZONTAL_ALT) {
      if (x - 1 >= 0 && x + 1 < MAP->mapWidth &&
          JOINS_RIGHT(CELL_AT(x - 1, y)) && JOINS_LEFT(CELL_AT(x + 1, y))) {
        return 1;
      }
    } else if (cell == CELL_VERTICAL || cell == CELL_VERTICAL_ALT) {
      if (y - 1 >= 0 && y + 1 < MAP->mapHeight &&
          JOINS_DOWN(CELL_AT(x, y - 1)) && JOINS_UP(CELL_AT(x, y + 1))) {
        return 1;
      }
    } else if (cell == CELL_LEFT_UP) {
      if (x - 1 >= 0 && y - 1 >= 0 && JOINS_RIGHT(CELL_AT(x - 1, y)) &&
          JOINS_DOWN(CELL_AT(x, y - 1))) {
        return 1;
      }
    } else if (cell == CELL_LEFT_DOWN) {
      if (x - 1 >= 0 && y + 1 < MAP->mapHeight &&
          JOINS_RIGHT(CELL_AT(x - 1, y)) && JOINS_UP(CELL_AT(x, y + 1))) {
        return 1;
      }
    } else if (cell == CELL_RIGHT_UP) {
      if (x + 1 < MAP->mapWidth && y - 1 >= 0 &&
          JOINS_LEFT(CELL_AT(x + 1, y)) && JOINS_DOWN(CELL_AT(x, y - 1))) {
        return 1;
      }
    } else if (cell == CELL_RIGHT_DOWN) {
      if (x + 1 < MAP->mapWidth && y + 1 < MAP->mapHeight &&
          JOINS_LEFT(CELL_AT(x + 1, y)) && JOINS_UP(CELL_AT(x, y + 1))) {
        return 1;
      }
    }
  }
  return 0;
}
