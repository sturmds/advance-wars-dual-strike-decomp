#include "sdk/snd.h"

void SND_StartSeq(u32 playerNo, u32 seqBase, u32 seqOffset, u32 bank)
{
    Sound_PostCommandArgs(0, playerNo, seqBase, seqOffset, bank);
}
