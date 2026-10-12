#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"

SoundSeqPlayer* SoundSeqPlayer_Acquire(SoundHandle* handle, int playerNo,
                                       int playerPrio);
void* SoundSeqPlayer_AttachHeap(int playerNo, SoundSeqPlayer* player);
int SoundArc_GetBank(int bankNo, int loadFlags, void* heap, int cache,
                     void** loadedOut);
int SoundArc_GetSeq(int seqNo, int loadFlags, void* heap, int cache,
                    void** loadedOut);
void func_0210547c(SoundSeqPlayer* player);
void SoundSeqPlayer_Start(SoundSeqPlayer* player, const void* seqData,
                          u32 offset, void* bank);
void func_02105814(SoundHandle* handle, int volume);
void SoundHandle_SetChannelPriority(SoundHandle* handle, int channelPrio);
void SoundHandle_SetSourceSeq(SoundHandle* handle, int seqNo);

/* Starts sequence seqNo of the current sound archive on handle: takes a
 * player, loads the bank and the sequence into its heap, then starts it.
 * Returns FALSE when no player is free or something does not load. */
BOOL SoundHandle_StartSeq(SoundHandle* handle, int playerNo, int bankNo,
                          int playerPrio, const SoundArcSeqInfo* info,
                          int seqNo) {
  SoundSeqPlayer* player;
  void* heap;
  SoundSeqArcFile* seq;
  void* bank;

  player = SoundSeqPlayer_Acquire(handle, playerNo, playerPrio);
  if (player == NULL) {
    return FALSE;
  }
  heap = SoundSeqPlayer_AttachHeap(playerNo, player);
  if (SoundArc_GetBank(bankNo, 6, heap, 0, &bank) != 0) {
    func_0210547c(player);
    return FALSE;
  }
  if (SoundArc_GetSeq(seqNo, 1, heap, 0, (void**)&seq) != 0) {
    func_0210547c(player);
    return FALSE;
  }
  SoundSeqPlayer_Start(player, (const u8*)seq + seq->dataOffset, 0, bank);
  func_02105814(handle, info->volume);
  SoundHandle_SetChannelPriority(handle, info->channelPrio);
  SoundHandle_SetSourceSeq(handle, seqNo);
  return TRUE;
}
