#include "types.h"

/* Texture size code 0 to 3 to the size in pixels (8 to 64); any other code
 * gives 0. */
u32 TexSizeT_ToPixels(u32 sizeCode) {
  switch (sizeCode) {
    case 0:
      return 8;
    case 1:
      return 16;
    case 2:
      return 32;
    case 3:
      return 64;
    default:
      return 0;
  }
}
