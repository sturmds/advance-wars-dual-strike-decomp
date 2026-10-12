#include "types.h"

/* Pixel width of a digit sprite of the title call number: 1 is 6, 4 is 14,
 * the others 12. */
int TitleCall_GetDigitWidth(int digit) {
  switch (digit) {
    case 1:
      return 6;
    case 4:
      return 14;
    default:
      return 12;
  }
}
