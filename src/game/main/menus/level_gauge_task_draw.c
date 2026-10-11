#include "types.h"
#include "game/level_gauge.h"

void func_02049b84(LevelGauge *gauges, int x, int y, int index, int frame);
void CoRankScreen_QueueNumber(int x, int y, int value);

/* Script step: draws the points left at y 174 and the gauge of each CO
 * (the second only when twoGauges is 1) at rest. */
void LevelGaugeTask_Draw(LevelGaugeTask *task)
{
    CoRankScreen_QueueNumber(0, 174, task->pointsLeft);
    func_02049b84(task->gauges, 40, 144, 0, -1);
    if (task->twoGauges == 1) {
        func_02049b84(task->gauges, 176, 144, 1, -1);
    }
}
