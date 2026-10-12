#include "types.h"
#include "game/spark_burst_task.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(SparkBurstTask* task);

/* (dx, dy) pairs. Needed to match: indexed flat, not as [n][2]. */
extern s8 gSparkBurstOffsets[SPARK_BURST_SPARKS * 2];
extern u8 data_027c0028;

/* One spark of a style: skipped once its flight is over, otherwise eased
 * out to its end point while its animation runs frames 0 to 5. */
#define SPARK_STEP(headStart, duration)                                      \
  if (task->frame + (headStart) > (duration)) {                              \
    continue;                                                                \
  }                                                                          \
  dx = Math_Ease(4, 0, gSparkBurstOffsets[i * 2], task->frame + (headStart), \
                 (duration));                                                \
  dy = Math_Ease(4, 0, gSparkBurstOffsets[i * 2 + 1],                        \
                 task->frame + (headStart), (duration));                     \
  frame = Math_Ease(4, 0, 5, task->frame + (headStart), (duration));         \
  break;

/* One frame of the burst: each spark flies from (x, y) to a point 60 pixels
 * away in its direction (gSparkBurstOffsets holds the end offsets, 10 degrees
 * apart). Its style picks a head start of 0, 2, 4 or 6 frames and a flight
 * of 32, 16, 24 or 8 frames. Resumes the script after frame 32. */
void SparkBurst_Update(SparkBurstTask* task) {
  int i;
  int dx;
  int dy;
  int frame;

  for (i = 0; i < SPARK_BURST_SPARKS; i++) {
    switch (task->sparkStyles[i]) {
      case 0:
        SPARK_STEP(0, 32)
      case 1:
        SPARK_STEP(0, 16)
      case 2:
        SPARK_STEP(0, 24)
      case 3:
        SPARK_STEP(0, 8)
      case 4:
        SPARK_STEP(2, 32)
      case 5:
        SPARK_STEP(2, 16)
      case 6:
        SPARK_STEP(2, 24)
      case 7:
        SPARK_STEP(2, 8)
      case 8:
        SPARK_STEP(4, 32)
      case 9:
        SPARK_STEP(4, 16)
      case 10:
        SPARK_STEP(4, 24)
      case 11:
        SPARK_STEP(4, 8)
      case 12:
        SPARK_STEP(6, 32)
      case 13:
        SPARK_STEP(6, 16)
      case 14:
        SPARK_STEP(6, 24)
      default:
        SPARK_STEP(6, 8)
    }
    Sprite_QueueAt(0, (task->x + dx) & 0x1ff, (task->y + dy) & 0xff,
                   &data_027c0028, 0x3000 | (0x194 + frame * 2), 0);
  }
  if (task->frame != 32) {
    task->frame++;
    return;
  }
  Task_ResumeScript(task);
}
