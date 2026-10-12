#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 Ov006_gShoalGraphicTable[];

int Ov006_IsLandNeighbor(
    int x,
    int y); /* guess: 1 when tile x, y is water (sea, shoal, reef, ...) */
int func_ov006_02363cb8(
    int x, int y); /* shoal neighbours: 8 above, 4 left, 2 right, 1 below */
int Ov006_IsBareShoal(int x, int y); /* 1 when tile x, y is shoal */

#define MAP (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Picks the graphic for a shoal at tile x, y from its eight neighbours.
 * Each water neighbour sets one bit (0x100 up-left through 1 down-right,
 * row by row; 0x10 is the tile itself and is left out of the lookup).
 * A table entry below 0 means no shoal fits here. Otherwise bits 9-14 name
 * a corner case that also looks at neighbouring shoals, and the low 9 bits
 * are the graphic. The case for 0x800 tests x > 0 before reading the tile
 * above (the other cases test y); maybe a slip in the original. */
s16 Ov006_GetShoalGraphic(int x, int y) {
  int mask = 0;
  int tile;
  int shoals;

  if (y > 0) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y - 1) << 8;
    }
    mask |= Ov006_IsLandNeighbor(x, y - 1) << 7;
    if (x < MAP->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y - 1) << 6;
    }
  }
  if (x > 0) {
    mask |= Ov006_IsLandNeighbor(x - 1, y) << 5;
  }
  mask |= Ov006_IsLandNeighbor(x, y) << 4;
  if (x < MAP->mapWidth - 1) {
    mask |= Ov006_IsLandNeighbor(x + 1, y) << 3;
  }
  if (y < MAP->mapHeight - 1) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y + 1) << 2;
    }
    mask |= Ov006_IsLandNeighbor(x, y + 1) << 1;
    if (x < MAP->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y + 1);
    }
  }

  tile = Ov006_gShoalGraphicTable[mask & ~0x10];
  if (tile < 0) {
    return tile;
  }
  switch (tile & 0x7e00) {
    case 0x400:
      if (x > 0) {
        tile = Ov006_IsBareShoal(x - 1, y) ? 243 : 182;
      } else {
        tile = 182;
      }
      break;
    case 0x200:
      if (x < MAP->mapWidth - 1) {
        tile = Ov006_IsBareShoal(x + 1, y) ? 242 : 183;
      } else {
        tile = 183;
      }
      break;
    case 0x800:
      if (x > 0) {
        tile = Ov006_IsBareShoal(x, y - 1) ? 210 : 182;
      } else {
        tile = 182;
      }
      break;
    case 0x4a00:
      if (x < MAP->mapWidth - 1) {
        tile = Ov006_IsBareShoal(x + 1, y) ? 109 : 143;
      } else {
        tile = 143;
      }
      break;
    case 0x4c00:
      if (x > 0) {
        tile = Ov006_IsBareShoal(x - 1, y) ? 110 : 143;
      } else {
        tile = 143;
      }
      break;
    case 0x5200:
      if (x < MAP->mapWidth - 1) {
        tile = Ov006_IsBareShoal(x + 1, y) ? 205 : 239;
      } else {
        tile = 239;
      }
      break;
    case 0x5400:
      if (x > 0) {
        tile = Ov006_IsBareShoal(x - 1, y) ? 206 : 239;
      } else {
        tile = 239;
      }
      break;
    case 0x2c00:
      if (y > 0) {
        tile = Ov006_IsBareShoal(x, y - 1) ? 207 : 146;
      } else {
        tile = 146;
      }
      break;
    case 0x3400:
      if (y < MAP->mapHeight - 1) {
        tile = Ov006_IsBareShoal(x, y + 1) ? 175 : 146;
      } else {
        tile = 146;
      }
      break;
    case 0x2a00:
      if (y > 0) {
        tile = Ov006_IsBareShoal(x, y - 1) ? 208 : 147;
      } else {
        tile = 147;
      }
      break;
    case 0x3200:
      if (y < MAP->mapHeight - 1) {
        tile = Ov006_IsBareShoal(x, y + 1) ? 176 : 147;
      } else {
        tile = 147;
      }
      break;
    case 0x4800:
      shoals = func_ov006_02363cb8(x, y) & 7;
      if (shoals == 6) {
        tile = 16;
      } else if (shoals == 4) {
        tile = 110;
      } else if (shoals == 2) {
        tile = 109;
      } else {
        tile = 143;
      }
      break;
    case 0x5000:
      shoals = func_ov006_02363cb8(x, y) & 14;
      if (shoals == 6) {
        tile = 80;
      } else if (shoals == 2) {
        tile = 205;
      } else if (shoals == 4) {
        tile = 206;
      } else {
        tile = 239;
      }
      break;
    case 0x2400:
      shoals = func_ov006_02363cb8(x, y) & 9;
      if (shoals == 9) {
        tile = 46;
      } else if (shoals == 8) {
        tile = 207;
      } else if (shoals == 1) {
        tile = 175;
      } else {
        tile = 146;
      }
      break;
    case 0x2200:
      shoals = func_ov006_02363cb8(x, y) & 9;
      if (shoals == 9) {
        tile = 50;
      } else if (shoals == 8) {
        tile = 208;
      } else if (shoals == 1) {
        tile = 176;
      } else {
        tile = 147;
      }
      break;
    case 0xc00:
      shoals = func_ov006_02363cb8(x, y);
      if (shoals == 4) {
        tile = 243;
      } else if (shoals == 8) {
        tile = 210;
      } else if (shoals == 12) {
        tile = 47;
      } else {
        tile = 182;
      }
      break;
    case 0xa00:
      shoals = func_ov006_02363cb8(x, y);
      if (shoals == 2) {
        tile = 242;
      } else if (shoals == 8) {
        if (x > 0 && y > 0 && Ov006_IsLandNeighbor(x - 1, y - 1)) {
          tile = 211;
        } else {
          tile = 183;
        }
      } else if (shoals == 10) {
        if (x > 0 && y > 0 && Ov006_IsLandNeighbor(x - 1, y - 1)) {
          tile = 49;
        } else {
          tile = 242;
        }
      } else {
        tile = 183;
      }
      break;
    case 0x1400:
      shoals = func_ov006_02363cb8(x, y);
      if (shoals == 4) {
        tile = 0x113;
      } else if (shoals == 1) {
        tile = 178;
      } else if (shoals == 5) {
        tile = 79;
      } else {
        tile = 214;
      }
      break;
    case 0x1200:
      shoals = func_ov006_02363cb8(x, y);
      if (shoals == 2) {
        tile = 0x112;
      } else if (shoals == 1) {
        tile = 179;
      } else if (shoals == 3) {
        tile = 81;
      } else {
        tile = 215;
      }
      break;
  }
  if (tile > 0) {
    tile &= 0x1ff;
  }
  return tile;
}
