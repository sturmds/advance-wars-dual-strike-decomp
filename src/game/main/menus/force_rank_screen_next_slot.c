#include "types.h"
#include "game/force_rank_screen.h"

void Sound_PlaySe(int id);

/* R button: selects the next skill slot, wrapping around. */
void ForceRankScreen_NextSlot(void)
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
    if (gForceRankScreen->unk_7b < slotCount - 1) {
        gForceRankScreen->unk_7b++;
    } else {
        gForceRankScreen->unk_7b = 0;
    }
}
