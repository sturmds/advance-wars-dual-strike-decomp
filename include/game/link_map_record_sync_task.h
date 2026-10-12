#ifndef GAME_LINK_MAP_RECORD_SYNC_TASK_H
#define GAME_LINK_MAP_RECORD_SYNC_TASK_H

#include "types.h"
#include "game/design_map.h"

/* Task that sends the design map records of a link battle's two maps to every
 * player, in chunks of 8 bytes (script gLinkMapRecordSyncScript: LinkMapRecordSync_Init sets it up,
 * LinkMapRecordSync_Update swaps the data). The block holds one DesignMapRecord per map:
 * the main map's first, then the second map's. Each map's record comes from the
 * link player who made it. The first 0x2c bytes belong to the task system. */
typedef struct LinkMapRecordSyncTask {
  u8 pad_00[0x2c];
  u8* sendBuffer;    /* this player's records of both maps, Mem_Alloc'd */
  u8* receiveBuffer; /* data_02291774 */
  u16 sentBytes;
  u8 pad_36[2];
  u16 receivedBytes;
  u16 blockSize; /* bytes to send: 2 records, or 1 when the second map is not a design map */
  u16 sendDone;       /* set when sentBytes reaches blockSize */
  u16 started;        /* set once the first chunk went out */
  u16 receiveStarted; /* set when the first chunk arrived */
  s8 senders
      [2]; /* per map: link player who made it, -1 when not a design map */
} LinkMapRecordSyncTask;

#endif
