#include "types.h"
#include "game/force_rank_screen.h"

void Sound_PlaySe(int id);

/* R button: selects the next skill set, wrapping around. */
void ForceRankScreen_NextSet(void)
{
    int setCount;

    if (gForceRankScreen->unk_7a < 2) {
        return;
    }
    Sound_PlaySe(0x10002);
    setCount = gForceRankScreen->unk_7a;
    if (setCount > 4) {
        setCount = 4;
    }
    if (gForceRankScreen->unk_7b < setCount - 1) {
        gForceRankScreen->unk_7b++;
    } else {
        gForceRankScreen->unk_7b = 0;
    }
}
