#include "types.h"

/* Map case (MapHeader.unk_26) that the map list of a kind holds. Kinds 1-8
 * are the lists of the single-map and two-map tabs; 0 and anything above 8
 * have no case. */
int MapSelect_ListKindToMapCase(int kind) {
  switch (kind) {
    case 1:
      return 2;
    case 3:
      return 3;
    case 2:
      return 4;
    case 4:
      return 5;
    case 5:
      return 6;
    case 6:
      return 7;
    case 7:
      return 9;
    case 8:
      return 10;
    default:
      return 0;
  }
}
