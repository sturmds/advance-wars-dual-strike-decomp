#include "types.h"

/* A second copy of TexSize_ToPixels. */
int TexSize_ToPixels_02028498(int sizeCode) {
  switch (sizeCode) {
    case 0:
      return 8;
    case 1:
      return 16;
    case 2:
      return 32;
    case 3:
      return 64;
    case 4:
      return 128;
    case 5:
      return 256;
    case 6:
      return 512;
    case 7:
      return 1024;
    default:
      return 0;
  }
}
