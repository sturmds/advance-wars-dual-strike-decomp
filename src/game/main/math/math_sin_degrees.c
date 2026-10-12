#include "types.h"

extern u16 gSinTable[]; /* sin 0 to 90 degrees, 16384 = 1.0 */

/* Sine of an angle in degrees (any integer), scaled so that 16384 is 1.0. */
s16 Math_SinDegrees(int degrees) {
  int half;

  if (degrees < 0) {
    do {
      degrees += 360;
    } while (degrees < 0);
  }
  if (degrees > 359) {
    do {
      degrees -= 360;
    } while (degrees > 359);
  }
  half = degrees;
  if (degrees > 179) {
    degrees -= 180;
  }
  if (degrees > 90) {
    degrees = 180 - degrees;
  }
  if (half <= 179) {
    return gSinTable[degrees];
  }
  return -gSinTable[degrees];
}
