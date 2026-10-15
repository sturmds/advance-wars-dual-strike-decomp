#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

/* Destroys the sound heap of a player heap and takes the player heap out of
 * use: a player that holds it loses its heap, otherwise it leaves the
 * unused list of its group. */
void SoundPlayerHeap_Destroy(SoundPlayerHeap* playerHeap) {
  if (playerHeap->heap == NULL) {
    return;
  }
  NNS_SndHeapDestroy(playerHeap->heap);
  if (playerHeap->player != NULL) {
    playerHeap->player->heap = NULL;
    return;
  }
  LinkList_Remove(&data_022a84a8[playerHeap->slotNo].heaps, playerHeap);
}
