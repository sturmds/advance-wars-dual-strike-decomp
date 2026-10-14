#include "types.h"
#include "game/owner_info.h"
#include "game/terrain.h"

extern TerrainIcon data_02166758[]; /* per terrain */
extern u8*
    data_ov000_022f6660[]; /* icon tile data, indexed by data_ov000_023503a0 */

/* The 8 tiles (256 bytes) of the icon of a terrain id. */
u8* Terrain_GetIconTiles(int terrain) {
  return data_ov000_022f6660[data_ov000_023503a0] +
         data_02166758[terrain].tiles * 256;
}
