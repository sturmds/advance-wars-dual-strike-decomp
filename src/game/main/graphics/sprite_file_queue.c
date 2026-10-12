#include "types.h"

const void* SpriteFile_GetCells(const void* file, int animation, int time);
void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a);

/* Queues the frame of animation at time from a sprite file in sprite list
 * list, with Sprite_Queue. Returns FALSE when the animation has ended. */
BOOL SpriteFile_Queue(const void* file, int animation, int time, int x, int y,
                      int attr, int tile, int list) {
  const void* cells = SpriteFile_GetCells(file, animation, time);

  if (cells != NULL) {
    Sprite_Queue(list, x, y, cells, attr, tile);
    return TRUE;
  }
  return FALSE;
}
