#include "sdk/snd_seq.h"

void SndSeq_FillCache(u8* address) {
  u32* source = (u32*)((u32)address & ~3);

  data_arm7_03808074.start = (u8*)source;
  data_arm7_03808074.end = (u8*)(source + 4);
  data_arm7_03808074.words[0] = source[0];
  data_arm7_03808074.words[1] = source[1];
  data_arm7_03808074.words[2] = source[2];
  data_arm7_03808074.words[3] = source[3];
}
