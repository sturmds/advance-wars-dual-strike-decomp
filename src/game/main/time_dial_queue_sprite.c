#include "types.h"

extern u8 gTimeDialSprites[];

int MainLoop_GetFrameCount(void);
BOOL SpriteFile_Queue(void* sprites, int frame, int animFrame, int x, int y,
                      int attr, int tile, int list);

/* Queues sprite `index` of the time dial's sprite file at (x, y), animated by
 * the frame count. */
void TimeDial_QueueSprite(int index, int x, int y) {
  SpriteFile_Queue(gTimeDialSprites, index, MainLoop_GetFrameCount(), x & 0x1ff,
                   y & 0xff, 0x1100, 0, 0);
}
