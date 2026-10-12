#include "types.h"
#include "game/tag_gauge_task.h"

extern u16 data_0229fb70[]; /* 16 colors the gauge cycles through */

void Gauge_DrawValueAndBar(int x, int y, int value, int frame);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u32 offset, u32 size);
void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(TagGaugeTask* task);

#define SE_GAUGE_TICK 236

/* Step of the tag firepower gauge script: counts the shown value up by 1 a
 * frame, with a tick sound on every even value, until it reaches the target,
 * then resumes the script. */
void TagGauge_CountUp(TagGaugeTask* task) {
  Gauge_DrawValueAndBar(TAG_GAUGE_X, TAG_GAUGE_Y, task->value, -1);
  Palette_Load(&data_0229fb70[(MainLoop_GetFrameCount() >> 2) & 15], 0x26c, 2);
  if (task->value == task->target) {
    Task_ResumeScript(task);
    return;
  }
  if ((task->value & 1) == 0) {
    Sound_PlaySe(SE_GAUGE_TICK);
  }
  task->value++;
}
