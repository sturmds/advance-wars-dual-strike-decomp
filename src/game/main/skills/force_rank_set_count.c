#include "types.h"

/* Number of Force Rank skill sets each CO has in save mode `mode`. */
int ForceRank_SetCount(int mode) {
  switch (mode) {
    case 2:
      return 4;
    default:
      return 1;
  }
}
