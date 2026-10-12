#include "types.h"

/* Weighted grey of a 5-bit RGB colour: (3 red + 6 green + blue) / 10,
 * packed as RGB555. Red and green get the grey value, blue gets half of
 * it. The /2*2/2 and *2/2 shapes are needed to match. */
u16 Rgb_ToGrey(u16 red, u16 green, u16 blue) {
  u16 grey = (red * 3 + green * 6 + blue) / 10;
  u16 blueGrey = grey / 2 * 2 / 2;
  u16 redGreenGrey = grey * 2 / 2;

  return (blueGrey << 10) + (redGreenGrey << 5) + redGreenGrey;
}
