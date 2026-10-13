#include "types.h"
#include "game/sound_handle.h"
#include "game/sound_heap.h"

void SoundPlayerHeap_Destroy(SoundPlayerHeap* playerHeap);
SoundHeap* SoundHeap_Create(u32 mem, u32 size);
void* SoundHeap_AllocItem(SoundHeap* heap, u32 size,
                          void (*onUnload)(void* payload, u32 arg1, u32 arg2,
                                           u32 arg3),
                          u32 arg3, u32 arg4);

/* Allocates a player heap of size bytes from heap and adds it to the unused
 * heaps of group slotNo. Returns FALSE when the memory does not suffice. */
BOOL SoundPlayerHeap_Create(int slotNo, SoundHeap* heap, u32 size) {
  SoundPlayerHeap* playerHeap = SoundHeap_AllocItem(
      heap, size + sizeof(SoundPlayerHeap),
      (void (*)(void*, u32, u32, u32))SoundPlayerHeap_Destroy, 0, 0);
  SoundHeap* playerSoundHeap;

  if (playerHeap == NULL) {
    return FALSE;
  }
  playerHeap->player = NULL;
  playerHeap->slotNo = slotNo;
  playerHeap->heap = NULL;
  playerSoundHeap = SoundHeap_Create((u32)(playerHeap + 1), size);
  if (playerSoundHeap == NULL) {
    return FALSE;
  }
  playerHeap->heap = playerSoundHeap;
  LinkList_Append(&data_022a84a8[slotNo].heaps, playerHeap);
  return TRUE;
}
