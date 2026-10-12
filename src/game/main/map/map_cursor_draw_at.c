#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_ov000_022c9750[];

BOOL Screen0_AllowsMode(int mode);
int MapCursor_GetAnimation(int* x, int* y, int style);
void Sprite3d_DrawAnimation(const VecFx32* pos, int animation);
int MainLoop_GetFrameCount(void);
BOOL SpriteFile_Queue(void* set, int index, int arg, int x, int y, int attr,
                      int tile, int list);

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Draws the map cursor sprite at map pixel position (x, y) in the given
 * style (MapCursor_GetAnimation picks the animation and moves the position).
 * On the 3D map screen it goes through Sprite3d_DrawAnimation (32 world
 * units per pixel); otherwise it is drawn as a 2D sprite relative to the view
 * scroll, and skipped when off screen. */
void MapCursor_DrawAt(int x, int y, int style) {
  int animation;
  VecFx32 pos;

  if (Screen0_AllowsMode(style)) {
    animation = MapCursor_GetAnimation(&x, &y, style);
    pos.x = (x << 13) / 256;
    pos.y = y * 0x6000 / 768;
    pos.z = 8;
    Sprite3d_DrawAnimation(&pos, animation);
    return;
  }
  x -= VIEW->scrollX;
  y -= VIEW->scrollY;
  animation = MapCursor_GetAnimation(&x, &y, style);
  if (x < 0 || y < 0 || x > 256 || y > 192) {
    return;
  }
  SpriteFile_Queue(data_ov000_022c9750, animation, MainLoop_GetFrameCount(),
                   x & 0x1ff, y & 0xff, 0x12be, 0, 1);
}
