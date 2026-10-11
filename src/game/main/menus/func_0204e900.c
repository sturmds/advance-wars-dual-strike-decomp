#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"

int Math_EaseInOut(int from, int to, int step, int steps);
void Task_ResumeScript(CampaignMapTask *task);

extern Unk_027c0280 *data_027c0280;

/* Script step: eases the scroll of the two map layers (the BGs of layers 3 and
 * 2) from (fromX, fromY) to (toX, toY) over 60 frames, then resumes the
 * script. */
void func_0204e900(CampaignMapTask *task)
{
    int bgA;
    int bgB;

    task->step++;
    bgA = data_027c0280->layer3Bg;
    bgB = data_027c0280->layer2Bg;
    data_027c0280->bgScrollX[bgA] = Math_EaseInOut(task->fromX, task->toX, task->step, 60);
    data_027c0280->bgScrollX[bgB] = data_027c0280->bgScrollX[bgA];
    bgA = data_027c0280->layer3Bg;
    bgB = data_027c0280->layer2Bg;
    data_027c0280->bgScrollY[bgA] = Math_EaseInOut(task->fromY, task->toY, task->step, 60);
    data_027c0280->bgScrollY[bgB] = data_027c0280->bgScrollY[bgA];
    if (task->step == 60) {
        task->step = 0;
        Task_ResumeScript(task);
    }
}
