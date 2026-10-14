#ifndef GAME_LINK_DATA_SYNC_TASK_H
#define GAME_LINK_DATA_SYNC_TASK_H

#include "types.h"
#include "game/design_map.h"

/* Fields of the tasks that swap a block of data between link players in
 * chunks of 8 bytes: the design maps (LinkDesignMapSync_Update) and the
 * map flags (LinkMapFlagsSync_Update).
 * The first 0x2c bytes belong to the task system (Task_Create). */
typedef struct LinkDataSyncTask {
  u8 pad_00[0x2c];
  u8* sendBuffer;     /* the design map rows to send (LinkDesignMapSync_Init) */
  u16 sentBytes;      /* how much of the block has been sent */
  u16 receivedBytes;  /* how much of each player's block has been filled */
  u16 sendDone;       /* set when sentBytes reaches the block size */
  u16 started;        /* set once the first chunk went out */
  u16 receiveStarted; /* set when the first chunk arrived */
} LinkDataSyncTask;

#define LINK_SYNC_CHUNK 8 /* bytes per packet */

#define LINK_DESIGN_MAP_BLOCK_SIZE \
  (LINK_DESIGN_MAPS_PER_PLAYER * sizeof(DesignMap))

#endif
