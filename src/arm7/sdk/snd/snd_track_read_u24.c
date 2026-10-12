#include "sdk/snd_seq.h"

u8 SndSeq_ReadByte(u8 *address);

u32 SndTrack_ReadU24(SndTrack *track)
{
    u32 value;
    u8 byte;

    value = SndSeq_ReadByte(track->cursor);
    track->cursor++;
    byte = SndSeq_ReadByte(track->cursor);
    track->cursor++;
    value |= byte << 8;
    byte = SndSeq_ReadByte(track->cursor);
    track->cursor++;
    value |= byte << 16;
    return value;
}
