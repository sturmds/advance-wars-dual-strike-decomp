#include "types.h"

void Sprite_Queue(int list, int x, int y, void *data, int attr, int tile);
extern u8 gCoPictureSprite[];

/* Queues a CO picture sprite (18 tiles from `tile`, palette in the top bits of
 * `attr`) with the shared sprite data gCoPictureSprite. */
void CoPicture_QueueSprite(int x, int y, int attr, int tile, int list)
{
    Sprite_Queue(list, x, y, gCoPictureSprite, attr, tile);
}
