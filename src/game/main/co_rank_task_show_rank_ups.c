#include "types.h"
#include "game/co_rank_task.h"

void CoRankTask_StartStaggeredItems(CoRankTask *task);
void CoRankScreen_QueueNumber(int x, int y, int value);
void LevelGauge_Draw(CoRankEntry *entries, int x, int y, int index, int frame);
BOOL CoRank_AreMarkersRunning(void);
void CoRankTask_StartRankPopInEvent(CoRankTask *task);
void CoRankTask_StartRank10Event(CoRankTask *task);
void CoRankTask_StartMaxRankEvent(CoRankTask *task);
BOOL EventScript_IsRunning(void);
void func_0204cc54(void);
void Task_ResumeScript(CoRankTask *task);

/* Script step: on frame 0 starts the rank-up items for COs whose frame is
 * set (CoRankTask_StartStaggeredItems), then draws the experience gained
 * and the rank of each CO for 16 frames. After that it waits while those
 * tasks run (CoRank_AreMarkersRunning) and while each once-only event script plays,
 * then adds the button of func_0204cc54 and resumes the script. */
void CoRankTask_ShowRankUps(CoRankTask *task)
{
    if (task->frame == 0) {
        CoRankTask_StartStaggeredItems(task);
    }
    CoRankScreen_QueueNumber(0, 174, task->experienceGain);
    LevelGauge_Draw(task->cos, 40, 144, 0, task->frame);
    if (task->hasBackCo == 1) {
        LevelGauge_Draw(task->cos, 176, 144, 1, task->frame);
    }
    if (task->frame == 16) {
        if (CoRank_AreMarkersRunning() == TRUE) {
            return;
        }
        CoRankTask_StartRankPopInEvent(task);
        if (EventScript_IsRunning()) {
            return;
        }
        CoRankTask_StartRank10Event(task);
        if (EventScript_IsRunning()) {
            return;
        }
        CoRankTask_StartMaxRankEvent(task);
        if (EventScript_IsRunning()) {
            return;
        }
        task->frame = 0;
        func_0204cc54();
        Task_ResumeScript(task);
    } else {
        task->frame++;
    }
}
