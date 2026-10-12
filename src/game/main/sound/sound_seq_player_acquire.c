#include "types.h"
#include "game/sound_handle.h"

extern SoundPlayerSlot data_022a84a8[]; /* sequence player groups */

void func_02105834(SoundHandle* handle);
void SoundSeqPlayer_StopNow(SoundSeqPlayer* player);
SoundSeqPlayer* SoundSeqPlayer_Alloc(int priority);
void SoundPlayerSlot_AddPlayer(SoundPlayerSlot* slot, SoundSeqPlayer* player);

/* Gets a sequence player of group slotNo for the handle. A player the handle
 * already points at is let go first. When the group is full, its lowest
 * priority player is stopped to make room. Returns NULL when that player's
 * priority is higher than the new one, or no player is left. */
SoundSeqPlayer* SoundSeqPlayer_Acquire(SoundHandle* handle, int slotNo,
                                       int priority) {
  SoundPlayerSlot* slot = &data_022a84a8[slotNo];
  SoundSeqPlayer* player;

  if (handle->player != NULL) {
    func_02105834(handle);
  }
  if (slot->players.count >= slot->maxPlayers) {
    player = LinkList_GetNext(&slot->players, NULL);
    if (player == NULL) {
      return NULL;
    }
    if (priority < player->priority) {
      return NULL;
    }
    SoundSeqPlayer_StopNow(player);
  }
  player = SoundSeqPlayer_Alloc(priority);
  if (player == NULL) {
    return NULL;
  }
  SoundPlayerSlot_AddPlayer(slot, player);
  player->handle = handle;
  handle->player = player;
  return player;
}
