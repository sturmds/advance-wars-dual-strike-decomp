#include "sdk/snd_seq.h"
#include "sdk/snd.h"

extern u8 data_arm7_0380807c[16]; /* the bytes of data_arm7_03808074 */

u8 SndSeq_ReadByte(u8* address) {
  if (address < data_arm7_03808074.start || address >= data_arm7_03808074.end) {
    SndSeq_FillCache(address);
  }
  return data_arm7_0380807c[address - data_arm7_03808074.start];
}
