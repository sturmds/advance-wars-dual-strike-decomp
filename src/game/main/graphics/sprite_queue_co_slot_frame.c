#include "types.h"

extern u8 gCoSlotFrameCells[];
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

/* Queues the frame sprite behind a CO picture slot on sprite list `list`:
 * palette slot 15, tile 0x358. attr adds OAM bits. */
void Sprite_QueueCoSlotFrame(int list, int x, int y, int attr) {
  Sprite_Queue(list, x, y, gCoSlotFrameCells, attr | 0xf358, 0);
}
