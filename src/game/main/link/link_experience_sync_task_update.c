#include "types.h"
#include "game/link_sync_task.h"
#include "game/link_data_sync_task.h"
#include "game/link_slots.h"

int func_0207bd94(void);
u8* Wireless_GetSharedData(int player);
u16 Wireless_GetChildAids(void);
int Link_FindSlotOfPlayer(int player);
void func_0207b48c(void);
void func_0207b4b0(u8* chunk);
void Task_ResumeScript(void* task);

/* Swaps CO experience with the other players, 8 bytes a frame: sends the
 * next chunk of task->experience and copies the chunk each player sent into
 * that player's row of data_0229deec (one u32 per CO, read by
 * Co_GetArmyExperience). The first chunk sent is a 1 marker. */
void LinkExperienceSyncTask_Update(LinkSyncTask* task) {
  int j;
  u8 chunk[LINK_SYNC_CHUNK];
  int i;
  u8* dst;
  u8* src;

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
        dst = (u8*)data_0229deec[Link_FindSlotOfPlayer(i) - 1];

        for (j = task->receivedBytes; j < task->receivedBytes + LINK_SYNC_CHUNK;
             j++) {
          if (j >= task->sentBytes) {
            break;
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
    u8* text = (u8*)task->experience;

    for (i = 0; i < LINK_SYNC_CHUNK; i++) {
      *out = text[task->sentBytes];
      task->sentBytes++;
      if (task->sentBytes == sizeof(data_0229deec[0])) {
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
