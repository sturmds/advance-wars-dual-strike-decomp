#include "types.h"
#include "game/math.h"

u16* Survival_GetMapList(int mode);
int Survival_GetMapCount(int mode);

/* Map id (an index into Ov000_gMapHeaders) at position index of the mode
 * variant's map list; the position wraps past the end of the list. */
int Survival_GetMapId(int mode, int index) {
  u16* maps = Survival_GetMapList(mode);

  return maps[HwMod32(index, Survival_GetMapCount(mode))];
}
