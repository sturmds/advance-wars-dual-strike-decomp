#include "sdk/snd_seq.h"

u8 SndSeq_ReadByte(u8 *address);

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
