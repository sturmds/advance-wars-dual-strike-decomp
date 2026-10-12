#include "types.h"

/* tile is an entry of data_ov003_02365d10: the low 5 bits are the terrain.
 * Returns 1 for terrain 6, 8, 10, 11, 14 and 22. What they have in common is
 * not known. */
int Ov003_IsTerrainSpecial(int tileInfo) {
  switch (tileInfo & 31) {
    case 6:
    case 8:
    case 10:
    case 11:
    case 14:
    case 22:
      return 1;
    default:
      return 0;
  }
}
