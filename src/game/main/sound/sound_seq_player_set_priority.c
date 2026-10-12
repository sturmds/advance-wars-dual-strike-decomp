#include "types.h"
#include "game/sound_handle.h"

extern LinkList data_022a805c; /* all sequence players, sorted by priority */

void SoundSeqPlayer_InsertActive(SoundSeqPlayer* player);
void SoundPlayerSlot_AddPlayer(SoundPlayerSlot* slot, SoundSeqPlayer* player);

/* Gives a sequence player a new priority and moves it to the matching place
 * in the owner's list and in the list of all players. */
void SoundSeqPlayer_SetPriority(SoundSeqPlayer* player, int priority) {
  SoundPlayerSlot* owner = player->ownerSlot;

  if (owner != NULL) {
    LinkList_Remove(&owner->players, player);
    player->ownerSlot = NULL;
  }
  LinkList_Remove(&data_022a805c, player);
  player->priority = priority;
  if (owner != NULL) {
    SoundPlayerSlot_AddPlayer(owner, player);
  }
  SoundSeqPlayer_InsertActive(player);
}
