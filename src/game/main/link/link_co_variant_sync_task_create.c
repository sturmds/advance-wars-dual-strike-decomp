#include "types.h"
#include "game/task.h"
#include "game/link_sync_task.h"

extern const TaskScriptCmd gLinkCoVariantSyncTaskScript[];

/* Starts the CO variant exchange as a child of parent; slotN is the player in link slot N. */
void LinkCoVariantSyncTask_Create(Task* parent, int slot0, int slot1, int slot2,
                                  u8 slot3) {
  LinkCoVariantSyncTask* task = (LinkCoVariantSyncTask*)Task_CreateChild(
      gLinkCoVariantSyncTaskScript, parent);

  task->players[0] = slot0;
  task->players[1] = slot1;
  task->players[2] = slot2;
  task->players[3] = slot3;
}
