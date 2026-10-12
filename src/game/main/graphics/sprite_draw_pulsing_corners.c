#include "types.h"

int MainLoop_GetFrameCount(void);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

extern u8 gCornerSpriteShape[];

/* Draws four corner sprites (tiles tile, +2, +4, +6) around the box at
 * (x, y) of size width x height, semi-transparent (OAM attribute 0 bit 10).
 * Over a 33-frame cycle the corners sit 2, 1, 0, then 1 pixels inside the
 * box, so they pulse. */
void Sprite_DrawPulsingCorners(int x, int y, int width, int height, int tile,
                               int palette) {
  int top;
  int inset;
  u8 phase;
  int right;
  int left;
  int bottom;

  phase = MainLoop_GetFrameCount() % 33;
  if (phase < 5) {
    inset = 2;
  } else if (phase < 9) {
    inset = 1;
  } else if (phase < 29) {
    inset = 0;
  } else {
    inset = 1;
  }
  top = (y + inset) | 0x400;
  left = x + inset;
  Oam_PutSprite(left, top, gCornerSpriteShape, tile | (palette << 12));
  right = x + width - inset;
  Oam_PutSprite(right, top, gCornerSpriteShape, (tile + 2) | (palette << 12));
  bottom = (y + height - inset) | 0x400;
  Oam_PutSprite(left, bottom, gCornerSpriteShape, (tile + 4) | (palette << 12));
  Oam_PutSprite(right, bottom, gCornerSpriteShape,
                (tile + 6) | (palette << 12));
}
