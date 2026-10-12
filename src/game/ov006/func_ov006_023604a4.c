#include "types.h"

int Ov006_GetNeighborTile(int x, int y);
int Ov006_GetLandMaskPiece(int x, int y);
int Ov006_GetRiverPiece(int x, int y);

/* Yes/no version of func_ov006_023624b4 for the tile at (x, y): 1 when it ends
 * up with a positive value, 0 for a negative Ov006_GetNeighborTile. The repeated
 * group tests are needed to match. */
int func_ov006_023604a4(int x, int y) {
  int first = Ov006_GetNeighborTile(x, y);
  int other;
  int group;

  if (first < 0) {
    return 0;
  }
  if (first != 0) {
    other = Ov006_GetRiverPiece(x, y);
    if (other == 0) {
      return 1;
    }
    if (other > 0) {
      group = first & 0xfe00;
      if (group == 0x4000) {
        if (group == 0x4000) {
          return 1;
        }
      }
      if (group == 0x2000) {
        if (group == 0x2000) {
          return 1;
        }
      }
      if (group == 0) {
        return 1;
      }
    }
  } else if (Ov006_GetNeighborTile(x, y) >= 0) {
    first = Ov006_GetLandMaskPiece(x, y);
    if (first != 0) {
      other = Ov006_GetRiverPiece(x, y);
      if (other != 0) {
        first = other;
      }
    }
    if (first > 0) {
      return 1;
    }
  }
  return 0;
}
