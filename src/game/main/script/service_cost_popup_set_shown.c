#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

/* Sets unk_38 (the last argument of ServiceCostPopup_Start) of the script slot that
 * is running to 1. */
void ServiceCostPopup_SetShown(void) {
  data_027c027c->scriptSlots[*data_027c027c->scriptSlotCurrent].unk_38 = 1;
}
