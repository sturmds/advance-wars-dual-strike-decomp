#include "types.h"
#include "game/task.h"
#include "game/link_data_sync_task.h"
#include "game/save_progress.h"
#include "game/link_slots.h"

u16 Wireless_GetChildAids(void);
u8* Wireless_GetSharedData(u16 slot);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(u8* chunk);
void Task_ResumeScript(Task* task);

/* needed to match: the original reaches gMapFlags (34 bytes, one bit per
 * map id) through gGameFlagsMid, which sits 10 bytes before it. */
#define MAP_FLAGS (gGameFlagsMid.value + 10)
#define MAP_FLAGS_SIZE 34

/* Each frame, sends the next 8 bytes of this player's map flags and ORs
 * the chunk each linked player sent into the local flags, so every player
 * ends up with the maps any of them has. Same protocol as
 * LinkDesignMapSync_Update. */
void LinkMapFlagsSync_Update(LinkDataSyncTask* task) {
  int i;
  u8 chunk[LINK_SYNC_CHUNK];
  int slot;
  u8* src;

  if (!func_0207bd94()) {
    return;
  }
  if (task->receiveStarted == 0) {
    if (*Wireless_GetSharedData(0) != 0) {
      task->receiveStarted = 1;
    }
  } else if (task->receiveStarted !=
             0) { /* needed to match: tests the flag again */
    for (slot = 0; slot < LINK_SLOT_COUNT; slot++) {
      if ((Wireless_GetChildAids() | 1) & (1 << slot)) {
        src = Wireless_GetSharedData((u16)slot);
        for (i = task->receivedBytes; i < task->receivedBytes + LINK_SYNC_CHUNK;
             i++) {
          if (i >= task->sentBytes) {
            break;
          }
          MAP_FLAGS[i] |= src[i - task->receivedBytes];
        }
      }
    }
    task->receivedBytes += LINK_SYNC_CHUNK;
  }
  if (task->sendDone != 0 && task->receivedBytes >= task->sentBytes) {
    task->sentBytes = 0;
    task->receivedBytes = 0;
    task->sendDone = 0;
    task->started = 0;
    task->receiveStarted = 0;
    func_0207b48c();
    Task_ResumeScript((Task*)task);
    return;
  }
  if (task->started == 0) {
    chunk[0] = 1;
    for (i = 1; i < LINK_SYNC_CHUNK; i++) {
      chunk[i] = 0;
    }
    task->started = 1;
  } else if (task->sendDone == 0) {
    for (i = 0; i < LINK_SYNC_CHUNK; i++) {
      chunk[i] = MAP_FLAGS[task->sentBytes];
      task->sentBytes++;
      if (task->sentBytes == MAP_FLAGS_SIZE) {
        task->sendDone = 1;
        i++;
        break;
      }
    }
    for (; i < LINK_SYNC_CHUNK; i++) {
      chunk[i] = 0;
    }
  } else {
    for (i = 0; i < LINK_SYNC_CHUNK; i++) {
      chunk[i] = 0;
    }
  }
  func_0207b4b0(chunk);
}
