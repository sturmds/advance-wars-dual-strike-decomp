#ifndef GAME_LINK_TEXT_SYNC_TASK_H
#define GAME_LINK_TEXT_SYNC_TASK_H

#include "types.h"

/* Fields of the task (LinkTextSyncTask_Update) that swaps the six reaction texts
 * (data_0229da8c, 198 bytes) between players in chunks of 8 bytes. The
 * first 0x2a bytes belong to the task system (Task_Create). */
typedef struct LinkTextSyncTask {
  u8 pad_00[0x2a];
  u16 sentBytes; /* how much of data_0229da8c has been sent */
  u16 receivedBytes; /* how much of each player's text in data_0229db54 has been filled */
  u16 sendDone;       /* set when sentBytes reaches 198 */
  u16 started;        /* set once the first chunk went out */
  u16 receiveStarted; /* set when the first chunk arrived */
} LinkTextSyncTask;

#endif
