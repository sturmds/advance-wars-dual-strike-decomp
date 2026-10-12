#include "types.h"
#include "game/tag_gauge_task.h"

extern u16 data_0229fb70[]; /* 16 colors the gauge cycles through */

void Gauge_DrawValueAndBar(int x, int y, int value, int frame);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u32 offset, u32 size);

/* Step of the tag firepower gauge script, after the pop-in step: draws the
 * finished gauge every frame and keeps its color cycling. The script never
 * resumes from here. */
void TagGauge_Hold(TagGaugeTask* task) {
  Gauge_DrawValueAndBar(TAG_GAUGE_X, TAG_GAUGE_Y, task->value, -1);
  Palette_Load(&data_0229fb70[(MainLoop_GetFrameCount() >> 2) & 15], 0x26c, 2);
}
