#ifndef GAME_CO_RANK_TASK_H
#define GAME_CO_RANK_TASK_H

#include "types.h"

/* Rank and experience of one CO as the rank screen shows them. */
typedef struct CoRankEntry {
  s8 rank;  /* Co_GetRank, 0-100 */
  u8 frame; /* pop-in frame of the rank digits, counts up to 255 */
  s16 exp;  /* experience toward the next rank: Co_GetExperience % 1000 */
} CoRankEntry;

/* Task of the screen at the end of a Survival run that shows player 1's
 * CO ranks (scripts near data_0215b1d8). The first 0x2a bytes belong to
 * the task system. */
typedef struct CoRankTask {
  u8 pad_00[0x2a];
  CoRankEntry cos[2]; /* front CO, back CO */
  u8 pad_32[0x34 - 0x32];
  s32 experienceBonus; /* Survival_GetExperienceBonus, set by CoRankScreen_Init */
  s32 experienceGain; /* run points plus bonus, at most 9999; drawn by CoRankScreen_QueueNumber */
  u8 pad_3c[0x44 - 0x3c];
  s16 frame; /* frame of the running animation step */
  u8 pad_46[0x4c - 0x46];
  s32 hasBackCo; /* 1: tag battle, the back CO is shown too */
} CoRankTask;

#endif
