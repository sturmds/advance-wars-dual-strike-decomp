#ifndef GAME_SPARK_BURST_TASK_H
#define GAME_SPARK_BURST_TASK_H

#include "types.h"

#define SPARK_BURST_SPARKS 36 /* one spark every 10 degrees */

/* Task of the script at data_0215c2fc (created by SparkBurst_Start): waits,
 * then draws SPARK_BURST_SPARKS sparks flying out from (x, y). The first
 * 0x2a bytes belong to the task system. */
typedef struct SparkBurstTask {
  u8 pad_00[0x2a];
  s16 frame;      /* frame of the running step */
  s16 waitFrames; /* frames SparkBurst_Wait waits before the burst */
  u8 pad_2e[0x30 - 0x2e];
  int x; /* burst centre on screen */
  int y;
  u8 sparkStyles
      [SPARK_BURST_SPARKS]; /* 0-15, random (SparkBurst_Init): head start and flight time */
} SparkBurstTask;

#endif
