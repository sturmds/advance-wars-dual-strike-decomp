#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

/* Same test as TimeLimit_GetRemaining, for the whole limit: stores the map
 * limit (timeLimitFrames) in total and returns TRUE when there is one. For the timed
 * Survival variants it stores the flag value 1, not a frame count. */
BOOL TimeLimit_GetTotal(u32 *total)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    BOOL survivalTimed;
    u32 mapLimit;
    BOOL timedVariant;

    *total = 0;
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    survivalTimed = FALSE;
    if (data_027c027c->unk_48[data_027c0284]->gameMode == 3) {
        timedVariant = TRUE;
        if (data_022a7b80.mode != 0) {
            if (data_022a7b80.mode != 3) {
                timedVariant = FALSE;
            }
        }
        if (timedVariant) {
            survivalTimed = TRUE;
        }
    }
    mapLimit = data_027c027c->unk_48[data_027c0284]->timeLimitFrames;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    if (survivalTimed) {
        *total = survivalTimed;
        return TRUE;
    }
    if (mapLimit != 0) {
        *total = mapLimit;
        return TRUE;
    }
    return FALSE;
}
