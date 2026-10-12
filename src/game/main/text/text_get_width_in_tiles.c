#include "types.h"

int Text_GetWidthWithCodes(const u8* text);

/* Width of text in 8-pixel tiles, rounded to the nearest tile. */
int Text_GetWidthInTiles(const u8* text) {
  return (Text_GetWidthWithCodes(text) + 6) / 8;
}
