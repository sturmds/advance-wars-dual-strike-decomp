#include "types.h"
#include "game/keypad.h"
#include "game/level_gauge.h"
#include "game/touch.h"

void Sound_PlaySe(int id);
void Task_ResumeScript(LevelGaugeTask *task);
void LevelGauge_Draw(LevelGauge *gauges, int x, int y, int index, int frame);
void CoRankScreen_QueueNumber(int a, int y, int value);

/* Each frame moves up to 4 points from pointsLeft into the gauges, every
 * full 1000 points making a level, plays a tick every other frame and
 * draws the points left and the gauges. A or a touch adds all the points
 * at once. Resumes the task's script when no points are left.
 * Needed to match: the carry tests read "> PER_LEVEL - 1", not ">=". */
void LevelGaugeTask_CountUp(LevelGaugeTask *task)
{
    int step;

    task->pointsLeft -= 4;
    step = 4;
    while (task->pointsLeft < 0) {
        task->pointsLeft++;
        step--;
    }
    task->gauges[0].points += step;
    if (task->twoGauges == 1) {
        task->gauges[1].points += step;
    }
    if (task->gauges[0].points > LEVEL_GAUGE_POINTS_PER_LEVEL - 1) {
        task->gauges[0].level++;
        task->gauges[0].points -= LEVEL_GAUGE_POINTS_PER_LEVEL;
        if (task->gauges[0].level <= LEVEL_GAUGE_MAX_LEVEL) {
            task->gauges[0].levelUpFrame = 1;
        }
    }
    if (task->twoGauges == 1 && task->gauges[1].points > LEVEL_GAUGE_POINTS_PER_LEVEL - 1) {
        task->gauges[1].level++;
        task->gauges[1].points -= LEVEL_GAUGE_POINTS_PER_LEVEL;
        if (task->gauges[1].level <= LEVEL_GAUGE_MAX_LEVEL) {
            task->gauges[1].levelUpFrame = 1;
        }
    }
    if (task->gauges[0].level >= LEVEL_GAUGE_MAX_LEVEL) {
        task->gauges[0].level = LEVEL_GAUGE_MAX_LEVEL;
        task->gauges[0].points = 0;
    }
    if (task->twoGauges == 1) {
        if (task->gauges[1].level >= LEVEL_GAUGE_MAX_LEVEL) {
            task->gauges[1].level = LEVEL_GAUGE_MAX_LEVEL;
            task->gauges[1].points = 0;
        }
    }
    if ((task->frame & 1) == 0) {
        Sound_PlaySe(0x1000c);
    }
    CoRankScreen_QueueNumber(0, 174, task->pointsLeft);
    LevelGauge_Draw(task->gauges, 40, 144, 0, -1);
    if (task->twoGauges == 1) {
        LevelGauge_Draw(task->gauges, 176, 144, 1, -1);
    }
    if (task->pointsLeft == 0) {
        task->frame = 0;
        Task_ResumeScript(task);
        return;
    }

    task->frame++;
    if (!(gKeypad->pressed & PAD_BUTTON_A)) {
        if (gTouch.phase != TOUCH_PRESSED || gTouch.active != 1) {
            return;
        }
    }
    task->gauges[0].points += task->pointsLeft;
    while (task->gauges[0].points > LEVEL_GAUGE_POINTS_PER_LEVEL - 1) {
        task->gauges[0].level++;
        task->gauges[0].points -= LEVEL_GAUGE_POINTS_PER_LEVEL;
        if (task->gauges[0].level <= LEVEL_GAUGE_MAX_LEVEL) {
            task->gauges[0].levelUpFrame = 1;
        }
    }
    if (task->twoGauges == 1) {
        task->gauges[1].points += task->pointsLeft;
        while (task->gauges[1].points > LEVEL_GAUGE_POINTS_PER_LEVEL - 1) {
            task->gauges[1].level++;
            task->gauges[1].points -= LEVEL_GAUGE_POINTS_PER_LEVEL;
            if (task->gauges[1].level <= LEVEL_GAUGE_MAX_LEVEL) {
                task->gauges[1].levelUpFrame = 1;
            }
        }
    }
    if (task->gauges[0].level >= LEVEL_GAUGE_MAX_LEVEL) {
        task->gauges[0].level = LEVEL_GAUGE_MAX_LEVEL;
        task->gauges[0].points = 0;
    }
    if (task->twoGauges == 1) {
        if (task->gauges[1].level >= LEVEL_GAUGE_MAX_LEVEL) {
            task->gauges[1].level = LEVEL_GAUGE_MAX_LEVEL;
            task->gauges[1].points = 0;
        }
    }
    task->pointsLeft = 0;
    task->frame = 0;
    Task_ResumeScript(task);
}
