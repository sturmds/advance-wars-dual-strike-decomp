#include "types.h"
#include "game/tag_gauge_task.h"

extern u16 data_0229fb70[];            /* 16 colors the gauge cycles through */

void Gauge_DrawValueAndBar(int x, int y, int value, int frame);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void *src, u32 offset, u32 size);
void Task_ResumeScript(TagGaugeTask *task);

#define POP_IN_FRAMES 16

/* Step of the tag firepower gauge script, after TagGauge_CountUp: draws the
 * final value with the digit pop-in for POP_IN_FRAMES frames, then marks the
 * gauge as done and resumes the script. */
void TagGauge_PopIn(TagGaugeTask *task)
{
    task->frame++;
    Gauge_DrawValueAndBar(TAG_GAUGE_X, TAG_GAUGE_Y, task->value, task->frame);
    Palette_Load(&data_0229fb70[(MainLoop_GetFrameCount() >> 2) & 15], 0x26c, 2);
    if (task->frame == POP_IN_FRAMES) {
        task->frame = 0;
        task->popInDone = TRUE;
        Task_ResumeScript(task);
        return; /* needed to match: moves the "mov r0, task" after the constant 1 */
    }
}
