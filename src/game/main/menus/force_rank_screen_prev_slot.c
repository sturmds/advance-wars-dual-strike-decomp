#include "types.h"
#include "game/force_rank_screen.h"

void Sound_PlaySe(int id);

/* L button: selects the previous skill slot, wrapping around. At most 4 slots
 * exist (unk_7a counts them); the button does nothing when unk_7a is below 2. */
void ForceRankScreen_PrevSlot(void)
{
    int slotCount;

    if (gForceRankScreen->unk_7a < 2) {
        return;
    }
    Sound_PlaySe(0x10002);
    slotCount = gForceRankScreen->unk_7a;
    if (slotCount > 4) {
        slotCount = 4;
    }
    if (gForceRankScreen->unk_7b != 0) {
        gForceRankScreen->unk_7b--;
    } else {
        gForceRankScreen->unk_7b = slotCount - 1;
    }
}
