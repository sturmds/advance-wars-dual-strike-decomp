#include "types.h"

extern u8 gMapFlags[]; /* one bit per map id */

/* Sets map mapId's bit in gMapFlags to value (0 or 1). Unlike Map_SetFlag,
 * it leaves the linked map's bit alone. */
void Map_SetSingleFlag(u32 mapId, int value) {
  int bit = 1 << (mapId & 7);
  gMapFlags[mapId >> 3] = (~bit & gMapFlags[mapId >> 3]) | (bit * value);
}
