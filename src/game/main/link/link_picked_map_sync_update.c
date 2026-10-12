#include "types.h"
#include "game/task.h"
#include "game/design_map.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"
#include "game/link_text_sync_task.h"

extern MenuState gMenuState;
extern DesignMapRecord data_02291774;

int func_0207a260(void);
u8* Wireless_GetSharedData(u16 slot);
int func_0207bd94(void);
void func_0207b48c(void);
void func_0207b4b0(u8* chunk);
void Task_ResumeScript(Task* task);

/* Two-player link menu: sends the map record picked in stage 2 from the
 * player numbered unk_631 to the other one, 8 bytes per frame. The other
 * player copies it into data_02291774 (LinkMenu_SaveReceivedMap stores it
 * later) and counts sentBytes up without sending data. The first chunk holds
 * only a 1 in byte 0. When all bytes are sent and received, resumes the
 * task's script. The task has the layout of LinkTextSyncTask. */
void LinkPickedMapSync_Update(LinkTextSyncTask* task) {
  u8* dest;
  int i;
  u8 chunk[8];
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
    if (data_0229fb30.unk_631 != func_0207a260()) {
      dest = (u8*)&data_02291774 + task->receivedBytes;
      src = Wireless_GetSharedData(data_0229fb30.unk_631);
      for (i = task->receivedBytes; i < task->receivedBytes + 8; i++, dest++) {
        if (i >= task->sentBytes) {
          break;
        }
        *dest = src[i - task->receivedBytes];
      }
    }
    task->receivedBytes += 8;
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
    for (i = 1; i < 8; i++) {
      chunk[i] = 0;
    }
    task->started = 1;
  } else if (task->sendDone == 0) {
    if (data_0229fb30.unk_631 == func_0207a260()) {
      src = (u8*)&LINK_MAPS[data_0229fb30.unk_632] + task->sentBytes;
      for (i = 0; i < 8; i++) {
        chunk[i] = *src;
        src++;
        task->sentBytes++;
        if (task->sentBytes == sizeof(DesignMapRecord)) {
          task->sendDone = 1;
          i++;
          break;
        }
      }
      for (; i < 8; i++) {
        chunk[i] = 0;
      }
    } else {
      for (i = 0; i < 8; i++) {
        task->sentBytes++;
        if (task->sentBytes == sizeof(DesignMapRecord)) {
          task->sendDone = 1;
          break;
        }
      }
      for (i = 0; i < 8; i++) {
        chunk[i] = 0;
      }
    }
  } else {
    for (i = 0; i < 8; i++) {
      chunk[i] = 0;
    }
  }
  func_0207b4b0(chunk);
}
