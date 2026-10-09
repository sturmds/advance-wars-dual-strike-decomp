#include "types.h"
#include "game/sound.h"

void func_021053a0(SoundSeqPlayer *player, int fadeFrames);

void NNS_SndPlayerStopSeq(SoundHandle *handle, int fadeFrames)
{
    func_021053a0(handle->player, fadeFrames);
}
