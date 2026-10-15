#include "types.h"
#include "game/link_sync_task.h"
#include "game/link_data_sync_task.h"
#include "game/link_slots.h"
#include "game/math.h"

int func_0207bd94(void);
u8* Wireless_GetSharedData(int player);
u16 Wireless_GetChildAids(void);
int Link_GetPlayerOfSlot(int slot);
void func_0207b48c(void);
void func_0207b4b0(u8* chunk);
void Task_ResumeScript(void* task);

u8 data_0229de6c[LINK_SYNC_ARMY_COUNT][2][16];

/* Swaps the equipped skills with the other players, 8 bytes a frame. Each
 * link slot owns 32 bytes of data_0229de6c; a byte is taken from the player in
 * that slot, or from player 0 when that player is not connected. */
void LinkSkillSyncTask_Update(LinkSyncTask* task) {
  int j;
  int i;
  u8 chunk[LINK_SYNC_CHUNK];
  u8* src;
  int owner;
  u8* dst;

  if (!func_0207bd94()) {
    return;
  }
  if (task->receiveStarted == 0) {
    if (*Wireless_GetSharedData(0) != 0) {
      task->receiveStarted = 1;
    }
  } else if (task->receiveStarted != 0) {
    for (i = 0; i < LINK_SLOT_COUNT; i++) {
      if ((Wireless_GetChildAids() | 1) & (1 << i)) {
        src = Wireless_GetSharedData((u16)i);
        dst = data_0229de6c[0][0];
        for (j = task->receivedBytes; j < task->receivedBytes + LINK_SYNC_CHUNK;
             j++) {
          if (j >= task->sentBytes) {
            break;
          }
          owner = Link_GetPlayerOfSlot(HwDiv32(j, 32) + 1);
          if ((Wireless_GetChildAids() | 1) & (1 << owner)) {
            if (i != owner) {
              continue;
            }
          } else if (i != 0) {
            continue;
          }
          dst[j] = src[j - task->receivedBytes];
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
    Task_ResumeScript(task);
    return;
  }
  if (task->started == 0) {
    chunk[0] = 1;
    for (i = 1; i < LINK_SYNC_CHUNK; i++) {
      chunk[i] = 0;
    }
    task->started = 1;
  } else if (task->sendDone == 0) {
    u8* out =
        chunk; /* needed to match: chunk[i] keeps the data pointer in the wrong register */
    u8* data = task->skills;

    for (i = 0; i < LINK_SYNC_CHUNK; i++) {
      *out = data[task->sentBytes];
      task->sentBytes++;
      if (task->sentBytes == 128) {
        task->sendDone = 1;
        i++;
        break;
      }
      out++;
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
