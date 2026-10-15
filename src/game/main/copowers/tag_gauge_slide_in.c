#include "types.h"
#include "game/tag_gauge_task.h"
#include "game/math.h"

extern u16 data_0229fb70[]; /* 16 colors the gauge cycles through */

void Gauge_DrawValueAndBar(int x, int y, int value, int frame);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u32 offset, u32 size);
void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(TagGaugeTask* task);

#define SLIDE_FRAMES 16
#define SLIDE_START_OFFSET 120
#define SE_GAUGE_SLIDE_IN 235

/* Step of the tag firepower gauge script: over SLIDE_FRAMES frames the gauge
 * (still at value 0) slides up from 120 pixels below its place. The color at
 * palette byte 0x26c cycles through data_0229fb70, one color every 4 frames. */
void TagGauge_SlideIn(TagGaugeTask* task) {
  task->frame++;
  Gauge_DrawValueAndBar(
      TAG_GAUGE_X,
      Math_Ease(4, SLIDE_START_OFFSET, 0, task->frame, SLIDE_FRAMES) +
          TAG_GAUGE_Y,
      task->value, -1);
  Palette_Load(&data_0229fb70[(MainLoop_GetFrameCount() >> 2) & 15], 0x26c, 2);
  if (task->frame == SLIDE_FRAMES) {
    Sound_PlaySe(SE_GAUGE_SLIDE_IN);
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
