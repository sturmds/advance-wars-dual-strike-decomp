#include "types.h"
#include "game/level_gauge.h"
#include "game/Unk_02088d04.h"
#include "game/math.h"

void Task_ResumeScript(LevelGaugeTask* task);
void LevelGauge_Draw(LevelGauge* gauges, int x, int y, int index, int frame);
void CoRankScreen_QueueNumber(int x, int y, int value);

/* One frame of a 16-frame slide in from the right: layer 1 and the gauges
 * move left by an eased 256 to 0 pixels. Resumes the task's script on the
 * last frame. */
void LevelGaugeTask_SlideIn(LevelGaugeTask* task) {
  int offset;

  task->frame++;
  offset = Math_Ease(4, 256, 0, task->frame, 16);
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = -offset;
  CoRankScreen_QueueNumber(0, 174, task->pointsLeft);
  LevelGauge_Draw(task->gauges, offset + 40, 144, 0, -1);
  if (task->twoGauges == 1) {
    LevelGauge_Draw(task->gauges, offset + 176, 144, 1, -1);
  }
  if (task->frame == 16) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
