#include "types.h"

/* Per-peer ring buffer of received packets with bit 7 set in the header: 7
 * peers of 98 bytes (4 slots of 24 bytes, then the read and write indexes).
 * Declared as byte arrays because each index is a symbol of its own. */
extern u8 data_0229cb4c[][98];
extern u8 data_0229cbac[][98];
extern u8 data_0229cbad[][98];

void Mem_CopyBytes(void* src, void* dest, int size);

/* Takes the oldest packet out of the peer's queue into dest. Returns 0 when the queue is empty. */
int Link_PopFlaggedPacket(int player, void* dest) {
  u8* readIndex;
  u8 index;

  if (player != 0) {
    player = (u8)(player - 1);
  }
  readIndex = data_0229cbac[player];
  index = *readIndex;
  if (index == data_0229cbad[player][0]) {
    return 0;
  }
  Mem_CopyBytes(&data_0229cb4c[player][index * 24], dest, 24);
  (*readIndex)++;
  if (*readIndex == 4) {
    *readIndex = 0;
  }
  return 1;
}
