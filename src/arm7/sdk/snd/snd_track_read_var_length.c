#include "sdk/snd_seq.h"

u8 func_arm7_038013a4(u8 *address);

u32 SndTrack_ReadVarLength(SndTrack *track)
{
    u32 value = 0;
    u8 byte;

    do {
        byte = func_arm7_038013a4(track->cursor);
        track->cursor++;
        value = (byte & 0x7F) | (value << 7);
    } while (byte & 0x80);
    return value;
}
