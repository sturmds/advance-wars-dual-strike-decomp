#include "types.h"
#include "game/link_sync_task.h"

void Mem_ZeroUnits(u32 ignored, void* dst, u32 size, u32 unitBits);
void* Mem_Alloc(u32 size);
int Co_GetSavedPaletteVariant(int coId);
int Co_GetSavedPictureVariant(int coId);
int Co_GetExperience(int coId);

/* Clears the received rows and fills the block to send with this player's
 * palette and picture choice and experience for each of the 28 COs. */
void LinkExperienceSyncTask_Init(LinkSyncTask* task) {
  int i;

  for (i = 0; i < 4; i++) {
    Mem_ZeroUnits(0, data_0229deec[i], sizeof(data_0229deec[i]), 8);
  }
  task->experience = Mem_Alloc(sizeof(data_0229deec[0]));
  for (i = 0; i < 28; i++) {
    task->experience[i].paletteVariant = Co_GetSavedPaletteVariant(i);
    task->experience[i].pictureVariant = Co_GetSavedPictureVariant(i);
    task->experience[i].experience = Co_GetExperience(i);
  }
  task->sentBytes = 0;
  task->receivedBytes = 0;
  task->sendDone = 0;
  task->started = 0;
  task->receiveStarted = 0;
}
