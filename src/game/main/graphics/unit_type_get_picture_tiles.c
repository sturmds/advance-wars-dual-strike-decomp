#include "types.h"
#include "game/owner_info.h"

extern u16 data_02166598[]; /* picture number of each unit type */
extern u8* data_ov000_022f664c
    []; /* picture tile data, indexed by data_ov000_023503a0 */

/* The 8 tiles (256 bytes) of the small picture of a unit type, as the info
 * panel and the Design Room show it. */
u8* UnitType_GetPictureTiles(int unitType) {
  return data_ov000_022f664c[data_ov000_023503a0] +
         data_02166598[unitType] * 256;
}
