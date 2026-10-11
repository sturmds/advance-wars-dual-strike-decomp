#include "types.h"
#include "game/script_slot.h"

void ZoomSpriteTask_Start(int tileX, int tileY, int variant);

/* Script callback of the service cost popup: when the popup is shown (unk_38),
 * starts a zoom sprite on the serviced unit's tile; unk_34 picks the variant. */
void ServiceCostPopup_StartZoom(ServiceCostPopupSlot *slot)
{
    if (slot->unk_38 == 0) {
        return;
    }
    ZoomSpriteTask_Start(slot->tileX, slot->tileY, slot->unk_34);
}
