#include "types.h"

/* Per-peer ring buffer of received packets with bit 7 clear in the header: 7
 * peers of 386 bytes (16 slots of 24 bytes, then the read and write indexes). */
extern u8 data_0229cdfc[7][386];
extern u8 data_0229cf7c[7][386];
extern u8 data_0229cf7d[7][386];

void Mem_CopyBytes(void* src, void* dest, int size);

/* Takes the oldest packet out of the peer's queue into dest. Returns 0 when the queue is empty. */
int Link_PopUnflaggedPacket(int player, void* dest) {
  u8* readIndex;
  u8 index;

  if (player != 0) {
    player = (u8)(player - 1);
  }
  readIndex = data_0229cf7c[player];
  index = *readIndex;
  if (index == data_0229cf7d[player][0]) {
    return 0;
  }
  Mem_CopyBytes(&data_0229cdfc[player][index * 24], dest, 24);
  (*readIndex)++;
  if (*readIndex == 16) {
    *readIndex = 0;
  }
  return 1;
}
