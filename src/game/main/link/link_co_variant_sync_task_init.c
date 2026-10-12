#include "types.h"
#include "game/link_sync_task.h"

int Co_GetSavedPaletteVariant(int coId);
int Co_GetSavedPictureVariant(int coId);

extern u8 data_0216069c[7];

/* Packs the saved palette and picture variant of the 7 COs in data_0216069c, two a byte, and clears the counters. */
void LinkCoVariantSyncTask_Init(LinkCoVariantSyncTask* task) {
  int i;
  u8* co = data_0216069c;

  for (i = 0; i < 7; i++, co++) {
    if ((i & 1) == 0) {
      task->variants[i >> 1].palette0 = Co_GetSavedPaletteVariant(*co);
      task->variants[i >> 1].picture0 = Co_GetSavedPictureVariant(*co);
    } else {
      task->variants[i >> 1].palette1 = Co_GetSavedPaletteVariant(*co);
      task->variants[i >> 1].picture1 = Co_GetSavedPictureVariant(*co);
    }
  }
  task->sentBytes = 0;
  task->receivedBytes = 0;
  task->sendDone = 0;
  task->started = 0;
  task->receiveStarted = 0;
}
