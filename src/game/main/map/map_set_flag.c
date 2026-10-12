#include "types.h"
#include "game/map_header.h"

extern u8 gMapFlags[]; /* one bit per map id */

/* Sets map mapId's bit in gMapFlags to value (0 or 1), and the same bit for
 * the map id in its header's variantMapIds[0]. */
void Map_SetFlag(u32 mapId, int value) {
  int bit;
  int linkedId;

  /* needed to match: the operands are in a different order in the two updates */
  bit = 1 << (mapId & 7);
  gMapFlags[mapId >> 3] = (~bit & gMapFlags[mapId >> 3]) | (bit * value);
  linkedId = Ov000_gMapHeaders[mapId].variantMapIds[0];
  bit = 1 << (linkedId & 7);
  gMapFlags[linkedId >> 3] = (gMapFlags[linkedId >> 3] & ~bit) | (value * bit);
}
