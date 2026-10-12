#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 data_ov006_0236cedc[];

int Ov006_IsSpecialCell(
    int x,
    int y); /* guess: tile x, y holds one of four ids (0xfc, 0xfd, 0x11c, 0x11d) */
int Ov006_IsLandNeighbor(
    int x,
    int y); /* guess: nonzero when the tile at x, y counts as a neighbour */

/* Picks a tile variant from the 3x3 block around tile x, y: builds a 9-bit
 * mask (bit 8 top-left ... bit 0 bottom-right, bit 4 the tile itself, always
 * set) of the tiles Ov006_IsLandNeighbor accepts, and looks it up in
 * data_ov006_0236cedc. Tiles off the map count as 0. Returns -1 when the
 * tile itself does not take part. */
int Ov006_GetNeighborTile(int x, int y) {
  int mask;

  if (Ov006_IsSpecialCell(x, y) != 0 || Ov006_IsLandNeighbor(x, y) == 0) {
    return -1;
  }
  mask = 0;
  if (y > 0) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y - 1) << 8;
    }
    mask |= Ov006_IsLandNeighbor(x, y - 1) << 7;
    if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y - 1) << 6;
    }
  }
  if (x > 0) {
    mask |= Ov006_IsLandNeighbor(x - 1, y) << 5;
  }
  mask |= 1 << 4;
  if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1) {
    mask |= Ov006_IsLandNeighbor(x + 1, y) << 3;
  }
  if (y < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      mask |= Ov006_IsLandNeighbor(x - 1, y + 1) << 2;
    }
    mask |= Ov006_IsLandNeighbor(x, y + 1) << 1;
    if (x < data_027c027c->unk_48[data_027c0284]->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsLandNeighbor(x + 1, y + 1);
    }
  }
  return data_ov006_0236cedc[mask];
}
