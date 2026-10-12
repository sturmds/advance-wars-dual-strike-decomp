#ifndef GAME_LINK_WAIT_TASK_H
#define GAME_LINK_WAIT_TASK_H

#include "types.h"

/* Fields of a task that waits for the link peer (LinkTask_WaitForPeer). The
 * first 0x4c bytes belong to the task system (Task_Create). */
typedef struct LinkWaitTask {
  u8 pad_00[0x4c];
  s16 waitFrames; /* frames spent waiting for the peer */
  u8 pad_4e[0x58 - 0x4e];
  u32 ackedAids; /* bit n set once child aid n sent an ack (LinkMenu_WaitForChildAcks) */
} LinkWaitTask;

#endif
