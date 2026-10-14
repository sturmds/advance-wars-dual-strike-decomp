#include "types.h"
#include "game/link_text_sync_task.h"
#include "game/link_slots.h"

void Mem_ZeroUnits(u32 ignored, void* dst, u32 size, u32 unitBits);
extern u8 data_0229db54[];

/* Clears the progress counters. The loop zeroes the same 8 bytes four times; the
 * original does this, as far as the object code shows. */
void LinkTextSyncTask_Reset(LinkTextSyncTask* task) {
  int i;

  for (i = 0; i < LINK_SLOT_COUNT; i++) {
    Mem_ZeroUnits(0, data_0229db54, 8, 8);
  }
  task->sentBytes = 0;
  task->receivedBytes = 0;
  task->sendDone = 0;
  task->started = 0;
  task->receiveStarted = 0;
}
