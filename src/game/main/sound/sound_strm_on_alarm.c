#include "types.h"
#include "game/sound_strm.h"

void SoundStrm_Refill(SoundStrm *strm, int status);

/* Alarm handler set up by SoundStrm_Setup: one block has played, refill it. */
void SoundStrm_OnAlarm(SoundStrm *strm)
{
    SoundStrm_Refill(strm, STRM_CALLBACK_REFILL);
}
