#ifndef GAME_SPLASH_TASK_H
#define GAME_SPLASH_TASK_H

#include "types.h"

/* Fields of the tasks of scripts data_02169ccc (the wait, fade and spawn
 * steps), data_02169c94 (spawns the letters) and data_02169cac (one letter).
 * The first 0x2a bytes belong to the task system (Task_Create). The name is a
 * guess; the purpose of the whole sequence is not known. */
typedef struct SplashTask {
    u8 pad_00[0x2a];
    s16 x;                             /* letter task: set from letterIndex */
    s16 y;                             /* letter task: 80 */
    s16 letterIndex;                   /* letter task: 0-3, from the spawner's lettersMade */
    s16 unk_30;
    u8 pad_32;
    u8 unk_33;
    u8 riseStep;                       /* letter task: height added next frame while rising, halved each frame */
    u8 unk_35;
    u8 holdFrames;                     /* letter task: frames left resting after the bounces */
    u8 unk_37;
    u16 waitFrames;                    /* frames counted by Splash_WaitUpdate */
    u8 pad_3a[0x54 - 0x3a];
    int fadeFrames;                    /* frames counted by Splash_FadeUpdate */
    u8 pad_58[0x64 - 0x58];
    s16 step;                          /* spawner task: 10 spawns the letters, 50 resumes the script */
    s16 lettersMade;
} SplashTask;

#endif
