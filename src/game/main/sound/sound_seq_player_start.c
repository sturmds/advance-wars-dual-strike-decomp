#include "types.h"
#include "game/sound_handle.h"

void SND_StartSeq(int hwChannel, const void* seqData, u32 offset, void* bank);
void func_0210fdfc(int hwChannel, u32 trackMask, u32 channelMask);
u32 SND_GetCurrentCommandTag(void);
void Sound_BlockUntilCommandsFree(int arg);
void SoundSeqPlayer_ResetState(SoundSeqPlayer* player);

/* Starts the sequence data on the player's hardware channel, restricts its
 * tracks to the group's channels when the group has a limit, and marks the
 * player as playing. */
void SoundSeqPlayer_Start(SoundSeqPlayer* player, const void* seqData,
                          u32 offset, void* bank) {
  SoundPlayerSlot* slot = player->ownerSlot;

  if (slot->trackChannelMask != 0) {
    Sound_BlockUntilCommandsFree(2);
  }
  SND_StartSeq(player->hwChannel, seqData, offset, bank);
  if (slot->trackChannelMask != 0) {
    func_0210fdfc(player->hwChannel, 0xffff, slot->trackChannelMask);
  }
  SoundSeqPlayer_ResetState(player);
  player->startTick = SND_GetCurrentCommandTag();
  player->state = SEQ_PLAYER_PLAYING;
}
