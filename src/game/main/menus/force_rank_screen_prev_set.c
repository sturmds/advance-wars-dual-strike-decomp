#include "types.h"
#include "game/force_rank_screen.h"

void Sound_PlaySe(int id);

/* L button: selects the previous skill set, wrapping around. At most 4 sets
 * exist; the button does nothing when unk_7a is below 2. */
void ForceRankScreen_PrevSet(void)
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
    if (gForceRankScreen->unk_7b != 0) {
        gForceRankScreen->unk_7b--;
    } else {
        gForceRankScreen->unk_7b = setCount - 1;
    }
}
