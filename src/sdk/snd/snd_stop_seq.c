#include "sdk/snd.h"

void SND_StopSeq(u32 playerNo) { Sound_PostCommandArgs(1, playerNo, 0, 0, 0); }
