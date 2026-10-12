#include "sdk/snd_seq.h"

u8 func_arm7_038013a4(u8 *address);

u16 SndTrack_ReadU16(SndTrack *track)
{
    u8 low;
    u8 high;

    low = func_arm7_038013a4(track->cursor);
    track->cursor++;
    high = func_arm7_038013a4(track->cursor);
    track->cursor++;
    return low | (high << 8);
}
