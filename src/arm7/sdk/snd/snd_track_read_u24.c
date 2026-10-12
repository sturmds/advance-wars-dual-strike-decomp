#include "sdk/snd_seq.h"
#include "sdk/snd.h"

u32 SndTrack_ReadU24(SndTrack* track) {
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
