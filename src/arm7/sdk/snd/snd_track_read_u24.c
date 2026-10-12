#include "sdk/snd_seq.h"

u8 func_arm7_038013a4(u8 *address);

u32 SndTrack_ReadU24(SndTrack *track)
{
    u32 value;
    u8 byte;

    value = func_arm7_038013a4(track->cursor);
    track->cursor++;
    byte = func_arm7_038013a4(track->cursor);
    track->cursor++;
    value |= byte << 8;
    byte = func_arm7_038013a4(track->cursor);
    track->cursor++;
    value |= byte << 16;
    return value;
}
