#include "types.h"
#include "game/sound_handle.h"

#define LINK_OFFSET(type, field) ((u16)(u32) & ((type*)0)->field)

SoundSeqPlayer gSoundSeqPlayers[SEQ_PLAYER_COUNT];
SoundPlayerSlot data_022a84a8[SEQ_PLAYER_SLOT_COUNT];

/* Sets up the lists of sequence players, puts every player in the free
 * list with its own hardware channel number, and sets every group up with a
 * limit of one player and full volume. */
void SoundSeqPlayer_InitAll(void) {
  int i;
  SoundSeqPlayer* player;
  SoundPlayerSlot* slot;

  LinkList_Init(&data_022a805c, LINK_OFFSET(SoundSeqPlayer, activeLink));
  LinkList_Init(&data_022a8050, LINK_OFFSET(SoundSeqPlayer, activeLink));
  player = gSoundSeqPlayers;
  for (i = 0; i < SEQ_PLAYER_COUNT; i++) {
    player->state = SEQ_PLAYER_FREE;
    player->hwChannel = i;
    LinkList_Append(&data_022a8050, player);
    player++;
  }
  slot = data_022a84a8;
  for (i = 0; i < SEQ_PLAYER_SLOT_COUNT; i++) {
    LinkList_Init(&slot->players, LINK_OFFSET(SoundSeqPlayer, slotLink));
    LinkList_Init(&slot->heaps, LINK_OFFSET(SoundPlayerHeap, link));
    slot->volume = 127;
    slot->maxPlayers = 1;
    slot->trackChannelMask = 0;
    slot++;
  }
}
