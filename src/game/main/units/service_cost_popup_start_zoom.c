#include "types.h"
#include "game/script_slot.h"

void ZoomSpriteTask_Start(int tileX, int tileY, int variant);

/* Script callback of the service cost popup: when the popup is visible,
 * starts a zoom sprite on the serviced unit's tile. */
void ServiceCostPopup_StartZoom(ServiceCostPopupSlot* slot) {
  if (slot->visible == 0) {
    return;
  }
  ZoomSpriteTask_Start(slot->tileX, slot->tileY, slot->zoomVariant);
}
