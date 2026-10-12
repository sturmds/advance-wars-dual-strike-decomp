#include "types.h"
#include "game/co_data.h"

extern u8 data_ov005_0236a48c[33];

/* Text id of an entry label: entries 0 and 28-30 have fixed texts, the others
 * show the name of the CO in data_ov005_0236a48c (bit 7 is a flag). */
u32 Ov005_GetCoListEntryName(int entry) {
  if (entry == 0) {
    return 0x80000031;
  }
  if (entry == 28) {
    return 0x80000033;
  }
  if (entry == 29) {
    return 0x80000034;
  }
  if (entry == 30) {
    return 0x80000032;
  }
  return gCoTable[data_ov005_0236a48c[entry] & 0x7f].name.id;
}
