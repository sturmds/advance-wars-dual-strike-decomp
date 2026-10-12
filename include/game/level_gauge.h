#ifndef GAME_LEVEL_GAUGE_H
#define GAME_LEVEL_GAUGE_H

#include "types.h"

#define LEVEL_GAUGE_POINTS_PER_LEVEL 1000
#define LEVEL_GAUGE_MAX_LEVEL 100

/* A level with points toward the next one, as LevelGauge_Draw draws it. */
typedef struct LevelGauge {
  s8 level; /* stops at LEVEL_GAUGE_MAX_LEVEL */
  u8 levelUpFrame; /* 0; 1 when points carry into a level, then counts frames up to 255 */
  s16 points; /* 0-999 */
} LevelGauge;

/* Task that pours pointsLeft into one or two gauges (LevelGaugeTask_CountUp).
 * A Task (game/task.h) seen another way: the fields below sit on Task
 * bytes such as waitFrames, unk_38, unk_44 and unk_4c. */
typedef struct LevelGaugeTask {
  u8 pad_00[0x2a];
  LevelGauge gauges[2];
  u8 pad_32[0x38 - 0x32];
  int pointsLeft;
  u8 pad_3c[0x44 - 0x3c];
  s16 frame; /* frames since the count started */
  u8 pad_46[0x4c - 0x46];
  int twoGauges; /* 1: gauges[1] fills too */
} LevelGaugeTask;

#endif
