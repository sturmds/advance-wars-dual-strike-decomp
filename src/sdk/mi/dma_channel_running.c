#include "types.h"

typedef struct Unk_0210f0a4_0 Unk_0210f0a4_0;

struct Unk_0210f0a4_0 {
  u8 pad_00[0x40000b0];
  int unk_40000b0;
};

int Dma_ChannelRunning(int a0) {
  return (u32)(((Unk_0210f0a4_0*)(((a0 * 3) + 2) << 2))->unk_40000b0 &
               -0x80000000) >>
         31;
}
