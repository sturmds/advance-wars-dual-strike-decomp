#ifndef GAME_LINK_SYNC_TASK_H
#define GAME_LINK_SYNC_TASK_H

#include "types.h"

/* Fields of the tasks that swap a block of data between players over the
 * link, 8 bytes a frame (LinkSkillSyncTask_Update: equipped skills, LinkExperienceSyncTask_Update: CO
 * experience). Each task uses one of the two data pointers. The first 0x2a
 * bytes belong to the task system (Task_Create). */
typedef struct LinkSyncTask {
    u8 pad_00[0x2c];
    u8 *experience;                    /* 112 bytes to send, allocated by the task's setup */
    u8 *skills;                        /* 128 bytes to send: 4 players x 2 COs x 16, filled by func_02081478 */
    u16 sentBytes;
    u16 receivedBytes;
    u16 sendDone;                      /* set when sentBytes reaches the block size */
    u16 started;                       /* set once the first chunk went out */
    u16 receiveStarted;                /* set when the first chunk arrived */
} LinkSyncTask;

#endif
