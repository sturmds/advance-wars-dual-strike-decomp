/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

extern char gIsdSpaces[];

/* Writes count spaces, 16 at a time, from a table of spaces. */
void Isd_WriteSpaces(IsdPrintTarget* target, u32 count) {
  u32 chunk;

  while (count != 0) {
    chunk = 16;
    if (count < 16) {
      chunk = count;
    }
    target->write(target, gIsdSpaces, chunk);
    count -= chunk;
  }
}
