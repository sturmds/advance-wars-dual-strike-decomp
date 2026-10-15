#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_IsNotSeaTile(int x, int y);
int Ov006_IsLandNeighbor(int x, int y);
int Ov006_IsSpecialCell(int x, int y);

extern s16 data_ov006_0236cedc
    []; /* indexed by a 3x3 neighbour mask, bit 8 top left to bit 0 bottom right */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* -1 for a sea, shoal or reef tile. Otherwise the map cell from a 3x3
 * mask of the land neighbours (as in Ov006_GetLandMaskPiece), with the
 * tile's own bit 4 set unless Ov006_IsSpecialCell accepts it, looked up in
 * the table Ov006_GetRiverPiece uses. Ov006_ResetNeighborsToSea calls it on
 * neighbours with a special cell. */
s16 Ov006_GetLandPieceIfNotSea(int x, int y) {
  int mask;

  if (!Ov006_IsNotSeaTile(x, y)) {
    return -1;
  }
  mask = 0;
  if (y > 0) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y - 1) << 8;
    }
    mask |= Ov006_IsLandNeighbor(x, y - 1) << 7;
    if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y - 1) << 6;
    }
  }
  if (x > 0) {
    mask |= Ov006_IsLandNeighbor(x - 1, y) << 5;
  }
  if (!Ov006_IsSpecialCell(x, y)) {
    mask |= 1 << 4;
  }
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    mask |= Ov006_IsLandNeighbor(x + 1, y) << 3;
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y + 1) << 2;
    }
    mask |= Ov006_IsLandNeighbor(x, y + 1) << 1;
    if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y + 1);
    }
  }
  return data_ov006_0236cedc[mask];
}
