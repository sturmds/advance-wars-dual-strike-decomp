#include "types.h"
#include "game/sound_handle.h"

void SoundSeqPlayer_StopNow(SoundSeqPlayer* player);
void SoundSeqPlayer_InsertActive(SoundSeqPlayer* player);

/* Takes a free sequence player and gives it the priority. With none free,
 * stops the player in use with the lowest priority and takes that one.
 * Returns NULL when that player's priority is higher than the new one. */
SoundSeqPlayer* SoundSeqPlayer_Alloc(int priority) {
  SoundSeqPlayer* player = LinkList_GetNext(&data_022a8050, NULL);

  if (player == NULL) {
    player = LinkList_GetNext(&data_022a805c, NULL);
    if (priority < player->priority) {
      return NULL;
    }
    SoundSeqPlayer_StopNow(player);
  }
  LinkList_Remove(&data_022a8050, player);
  player->priority = priority;
  SoundSeqPlayer_InsertActive(player);
  return player;
}
