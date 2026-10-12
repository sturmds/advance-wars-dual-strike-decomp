#include "types.h"

/* Maps 41-44 show in the map select list only once their bit in gMapFlags
 * is set (MapSelect_IsMapPickable). */
int Map_NeedsUnlockFlag(int mapId) {
  return ((u32)(mapId - 41) <= (u32)3) ? 1 : 0;
}
