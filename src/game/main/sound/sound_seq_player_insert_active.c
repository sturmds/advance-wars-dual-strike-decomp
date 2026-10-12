#include "types.h"
#include "game/sound_handle.h"

extern LinkList data_022a805c; /* sequence players in use, sorted by priority */

/* Puts a player in the list of players in use, after the players of equal
 * or lower priority. */
void SoundSeqPlayer_InsertActive(SoundSeqPlayer* player) {
  SoundSeqPlayer* next = LinkList_GetNext(&data_022a805c, NULL);

  if (next != NULL) {
    do {
      if (player->priority < next->priority) {
        break;
      }
      next = LinkList_GetNext(&data_022a805c, next);
    } while (next != NULL);
  }
  LinkList_InsertBefore(&data_022a805c, next, player);
}
