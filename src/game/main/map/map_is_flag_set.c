#include "types.h"

extern u8 gMapFlags[]; /* one bit per map id */

/* Returns nonzero if map mapId's bit in gMapFlags is set. */
u8 Map_IsFlagSet(u32 mapId) {
  return (1 << (mapId & 7)) & gMapFlags[mapId >> 3];
}
