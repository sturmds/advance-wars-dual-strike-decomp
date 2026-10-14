#include "types.h"
#include "game/Unk_02088d04.h"

u8* Palette_GetBlock(int index, int player);
void TransferQueue_Add(const void* src, void* dst, u32 size);

extern u8 data_ov006_0236d644[];

/* Queues the copy of palette block `index` to dest; nothing happens when enable is 0. Block 14 takes its first
 * 128 bytes from data_ov006_0236d644, a .bss buffer of ov006 that no code writes, so they are zero, and the
 * rest from the block. Every caller is in ov006, so ov006 is loaded. */
void Palette_QueueBlockCopy(int index, int player, u8* dest, int enable) {
  if (!enable) {
    return;
  }
  if (index == 14) {
    TransferQueue_Add(data_ov006_0236d644, dest, 128);
    TransferQueue_Add(Palette_GetBlock(index, player) + 128, dest + 128, 128);
  } else {
    TransferQueue_Add(Palette_GetBlock(index, player), dest,
                      PALETTE_BLOCK_SIZE);
  }
}
