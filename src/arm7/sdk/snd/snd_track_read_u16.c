#include "sdk/snd_seq.h"

u8 SndSeq_ReadByte(u8 *address);

u16 SndTrack_ReadU16(SndTrack *track)
{
    u8 low;
    u8 high;

    low = SndSeq_ReadByte(track->cursor);
    track->cursor++;
    high = SndSeq_ReadByte(track->cursor);
    track->cursor++;
    return low | (high << 8);
}
