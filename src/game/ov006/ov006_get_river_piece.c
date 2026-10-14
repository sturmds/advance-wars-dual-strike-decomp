#include "types.h"
#include "game/Unk_02080440.h"

BOOL Ov006_CountsAsRiver(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 data_ov006_0236cedc
    []; /* indexed by a 3x3 neighbour mask, bit 8 top left to bit 0 bottom right */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Map cell for tile x, y as a river piece: builds a 3x3 mask of the
 * neighbours that count as river (the tile itself is bit 4 and stays
 * clear; neighbours off the map count as no river) and looks it up in
 * data_ov006_0236cedc. */
s16 Ov006_GetRiverPiece(int x, int y) {
  int mask = 0;

  if (y > 0) {
    if (x > 0) {
      mask |= Ov006_CountsAsRiver(x - 1, y - 1) << 8;
    }
    mask |= Ov006_CountsAsRiver(x, y - 1) << 7;
    if (x < STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_CountsAsRiver(x + 1, y - 1) << 6;
    }
  }
  if (x > 0) {
    mask |= Ov006_CountsAsRiver(x - 1, y) << 5;
  }
  if (x < STATE->unk_1c4->mapWidth - 1) {
    mask |= Ov006_CountsAsRiver(x + 1, y) << 3;
  }
  if (y < STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      mask |= Ov006_CountsAsRiver(x - 1, y + 1) << 2;
    }
    mask |= Ov006_CountsAsRiver(x, y + 1) << 1;
    if (x < STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_CountsAsRiver(x + 1, y + 1);
    }
  }
  return data_ov006_0236cedc[mask];
}
