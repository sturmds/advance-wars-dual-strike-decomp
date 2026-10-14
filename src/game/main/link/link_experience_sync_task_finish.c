#include "types.h"
#include "game/link_sync_task.h"

LinkCoRecord data_0229deec[LINK_SYNC_ARMY_COUNT][LINK_SYNC_CO_COUNT];

u16 Wireless_GetChildAids(void);
int Link_FindSlotOfPlayer(int player);
void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);
void Mem_Free(void* ptr);

/* Ends the experience sync. A player who is not connected gets a copy of player 0's received
 * row, as in the skill sync. Then frees the block that was sent. */
void LinkExperienceSyncTask_Finish(LinkSyncTask* task) {
  int i;

  for (i = 0; i < 4; i++) {
    if (!((1 << i) & (Wireless_GetChildAids() | 1))) {
      Mem_CopyUnits(data_0229deec[Link_FindSlotOfPlayer(0) - 1],
                    data_0229deec[Link_FindSlotOfPlayer(i) - 1], 112, 8);
    }
  }
  Mem_Free(task->experience);
}
