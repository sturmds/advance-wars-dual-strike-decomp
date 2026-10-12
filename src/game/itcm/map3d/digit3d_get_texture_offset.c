#include "types.h"

/* Texture offset of the image for digit (0-9), 32 bytes per image. With
 * bit 0 of flags set it is the image after the digits (offset 320) for any
 * digit. */
int Digit3d_GetTextureOffset(int digit, u32 flags) {
  if (flags & 1) {
    return 320;
  }
  return digit << 5;
}
