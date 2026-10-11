#include "sdk/snd.h"

void SND_InvalidateBankData(u32 start, u32 end)
{
    Sound_PostCommandArgs(29, start, end, 0, 0);
}
