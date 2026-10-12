#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_IsLandNeighbor(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 data_ov006_0236cadc
    [512]; /* indexed by a 3x3 neighbour mask, bit 8 top left to bit 0 bottom right */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Map cell for tile x, y from its land neighbours: builds a 3x3 mask of
 * the neighbours Ov006_IsLandNeighbor accepts, with the tile's own bit 4
 * always set and neighbours off the map left clear, and looks it up in
 * data_ov006_0236cadc. Ov006_GetRiverPiece does the same with river
 * neighbours and another table. */
s16 Ov006_GetLandMaskPiece(int x, int y) {
  int mask = 0;

  if (y > 0) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y - 1) << 8;
    }
    mask |= Ov006_IsLandNeighbor(x, y - 1) << 7;
    if (x < STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y - 1) << 6;
    }
  }
  if (x > 0) {
    mask |= Ov006_IsLandNeighbor(x - 1, y) << 5;
  }
  mask |= 1 << 4;
  if (x < STATE->unk_1c4->mapWidth - 1) {
    mask |= Ov006_IsLandNeighbor(x + 1, y) << 3;
  }
  if (y < STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y + 1) << 2;
    }
    mask |= Ov006_IsLandNeighbor(x, y + 1) << 1;
    if (x < STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y + 1);
    }
  }
  return data_ov006_0236cadc[mask];
}
