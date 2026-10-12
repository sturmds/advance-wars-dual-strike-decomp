#include "types.h"
#include "game/co_data.h"

extern u32 gLinkPlay;

int Co_GetArmyPictureVariantRaw(
    int army, int coId); /* picture variant of this CO in this army's colours */

/* Picture variant to show for a CO and army. Bit 7 of coId, or
 * gLinkPlay being 0, means army 0. Ids past the playable COs give 0. */
int Co_GetArmyPictureVariant(int coId, int army) {
  int id;

  if (gLinkPlay == 0 || (coId & 0x80)) {
    army = 0;
  }
  id = coId & 0x7f;
  if (id >= CO_COUNT) {
    return 0;
  }
  return Co_GetArmyPictureVariantRaw(army, id);
}
