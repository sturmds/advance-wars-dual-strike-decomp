#include "types.h"
#include "game/co_rank_task.h"

void func_02002620(int x, int y, int arg2, int arg3, int list, BOOL flip);

/* Script step: draws the portrait of the front CO at x 64 and, in a tag
 * battle, the back CO at x 192, mirrored. */
void CoRankTask_DrawPortraits(CoRankTask *task)
{
    func_02002620(64, 160, 0x6800, 0x400, 10, FALSE);
    if (task->hasBackCo == 1) {
        func_02002620(192, 160, 0x7800, 0x5b0, 11, TRUE);
    }
}
