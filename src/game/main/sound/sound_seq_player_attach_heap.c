#include "types.h"
#include "game/sound_handle.h"

void SoundHeap_Reset(void* heap);

/* Gives the player an unused heap of group slotNo, emptied for the new
 * sound. Returns the heap, or NULL when the group has none left. */
void* SoundSeqPlayer_AttachHeap(int slotNo, SoundSeqPlayer* player) {
  SoundPlayerSlot* slot = &data_022a84a8[slotNo];
  SoundPlayerHeap* playerHeap = LinkList_GetNext(&slot->heaps, NULL);

  if (playerHeap == NULL) {
    return NULL;
  }
  LinkList_Remove(&slot->heaps, playerHeap);
  playerHeap->player = player;
  player->heap = playerHeap;
  SoundHeap_Reset(playerHeap->heap);
  return playerHeap->heap;
}
