#include "types.h"
#include "game/sound_handle.h"

LinkList data_022a805c;
LinkList data_022a8050;

/* Takes a sequence player out of use: detaches its handle, group and heap
 * and puts it back in the list of free players. */
void SoundSeqPlayer_Release(SoundSeqPlayer* player) {
  SoundHandle* handle = player->handle;
  SoundPlayerSlot* slot;

  if (handle != NULL) {
    handle->player = NULL;
    player->handle = NULL;
  }
  slot = player->ownerSlot;
  LinkList_Remove(&slot->players, player);
  player->ownerSlot = NULL;
  if (player->heap != NULL) {
    LinkList_Append(&slot->heaps, player->heap);
    player->heap->player = NULL;
    player->heap = NULL;
  }
  LinkList_Remove(&data_022a805c, player);
  LinkList_Append(&data_022a8050, player);
  player->state = SEQ_PLAYER_FREE;
}
