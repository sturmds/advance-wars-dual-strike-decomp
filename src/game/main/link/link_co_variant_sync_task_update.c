#include "types.h"
#include "game/link_sync_task.h"

int func_0207bd94(void);
u8* Wireless_GetSharedData(int player);
u16 Wireless_GetChildAids(void);
void Co_SetArmyPictureVariant(int army, int coId, int variant);
void Co_SetArmyPaletteVariant(int army, int coId, int variant);
void func_0207b48c(void);
void func_0207b4b0(u8* chunk);
void Task_ResumeScript(void* task);

extern u8 data_0216069c[]; /* the 7 CO ids whose variants are sent */

/* Swaps the palette and picture variants of the COs in data_0216069c with the
 * other players and stores what player players[i] sent as army i + 1's. */
void LinkCoVariantSyncTask_Update(LinkCoVariantSyncTask* task) {
  LinkCoVariantPair* src;
  int j;
  int i; /* needed to match: one variable for the link slot and the chunk loops */
  u8 chunk[8];
  int player;

  if (!func_0207bd94()) {
    return;
  }
  if (task->receiveStarted == 0) {
    if (*Wireless_GetSharedData(0) != 0) {
      task->receiveStarted = 1;
    }
  } else if (task->receiveStarted != 0) {
    for (player = 0; player < 8; player++) {
      if ((Wireless_GetChildAids() | 1) & (1 << player)) {
        for (i = 0; i < 4; i++) {
          if (player == task->players[i]) {
            break;
          }
        }
        if (i == 4) {
          continue;
        }
        src = (LinkCoVariantPair*)Wireless_GetSharedData((u16)player);
        for (j = task->receivedBytes; j < task->receivedBytes + 8; j++, src++) {
          if (j >= task->sentBytes) {
            break;
          }
          Co_SetArmyPaletteVariant(i + 1, data_0216069c[j * 2], src->palette0);
          Co_SetArmyPictureVariant(i + 1, data_0216069c[j * 2], src->picture0);
          if (j * 2 + 1 < 7) {
            Co_SetArmyPaletteVariant(i + 1, data_0216069c[j * 2 + 1],
                                     src->palette1);
            Co_SetArmyPictureVariant(i + 1, data_0216069c[j * 2 + 1],
                                     src->picture1);
          }
        }
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
    Task_ResumeScript(task);
    return;
  }
  if (task->started == 0) {
    chunk[0] = 1;
    for (i = 1; i < 8; i++) {
      chunk[i] = 0;
    }
    task->started = 1;
  } else if (task->sendDone == 0) {
    u8* out =
        chunk; /* needed to match: chunk[i] keeps the data pointer in the wrong register */
    u8* data = (u8*)task->variants;

    for (i = 0; i < 8; i++) {
      *out = data[task->sentBytes];
      task->sentBytes++;
      if (task->sentBytes == 4) {
        task->sendDone = 1;
        i++;
        break;
      }
      out++;
    }
    for (; i < 8; i++) {
      chunk[i] = 0;
    }
  } else {
    for (i = 0; i < 8; i++) {
      chunk[i] = 0;
    }
  }
  func_0207b4b0(chunk);
}
