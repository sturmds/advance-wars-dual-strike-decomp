#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/design_map.h"

extern DesignMapRecord data_02291774;

int func_0207a260(void);
void DesignMap_SaveRecordToSlot(int slot, DesignMapRecord *record);

/* Saves the map received in data_02291774 into the slot of the button the other player picked (unk_633),
 * unless this player's number equals the stage 1 pick (unk_631). */
void LinkMenu_SaveReceivedMap(void)
{
    if (data_0229fb30.unk_631 == func_0207a260()) {
        return;
    }
    DesignMap_SaveRecordToSlot(data_0229fb30.unk_633, &data_02291774);
}
