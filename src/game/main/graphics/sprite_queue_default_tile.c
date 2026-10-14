#include "types.h"

u8 data_02151d10[40] = {
#include "data/main/data_02151d10.inc.c"
};
u8 data_02151d38[52] = {
#include "data/main/data_02151d38.inc.c"
};
/* byte offset of each pixel inside an 8x8 tile */
u8 gTile4bppPixelOffsets[64] = {
#include "data/main/gTile4bppPixelOffsets.inc.c"
};
u8 gCoPortraitSprite[76] = {
#include "data/main/gCoPortraitSprite.inc.c"
};
u8 data_02151df8[76] = {
#include "data/main/data_02151df8.inc.c"
};

void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a);

/* Sprite_QueueDefault with a tile number. */
void Sprite_QueueDefaultTile(int x, int y, int attr, int tile, int list) {
  Sprite_Queue(list, x | (u16)(x & 0xfe00), y | (u16)(y & 0xff00),
               data_02151d10, attr, tile);
}
