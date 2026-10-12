#ifndef GAME_FORCE_RANK_TASK_H
#define GAME_FORCE_RANK_TASK_H

#include "types.h"

/* Task made by VsSetup_StartForceRank for the Force Rank screen of one CO. The
 * first 0x4e bytes belong to the task system (Task_Create). */
typedef struct ForceRankTask {
  u8 pad_00[0x4e];
  s16 forceRankSet; /* 0-3, the CO's set to edit */
  u8 pad_50[2];
  u16 co; /* CO id */
} ForceRankTask;

#endif
