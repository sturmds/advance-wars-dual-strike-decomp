#include "types.h"

/* Texture width in texels for a size index (0-3: 8 to 64). */
u32 Battle_GetTexWidth(u32 index) {
  switch (index) {
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
