#include "sdk/snd_seq.h"
#include "sdk/snd.h"

u32 SndTrack_ReadVarLength(SndTrack *track)
{
    u32 value = 0;
    u8 byte;

    do {
        byte = SndSeq_ReadByte(track->cursor);
        track->cursor++;
        value = (byte & 0x7F) | (value << 7);
    } while (byte & 0x80);
    return value;
}
