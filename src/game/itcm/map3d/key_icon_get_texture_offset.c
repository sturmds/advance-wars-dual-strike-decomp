#include "types.h"
#include "game/keypad.h"

int MainLoop_GetFrameCount(void);

/* Texture offset of the image for a set of held keys: a D-pad direction
 * (3, 7, 11, 15), plus 1 for B and 2 for Select, or 19 for Start. The image
 * set changes every 16 frames, 20 images per set, starting at image 11.
 * -1 when none of these keys is held. */
int KeyIcon_GetTextureOffset(u32 keys) {
  int direction = -1;
  int frame;

  if (keys & PAD_KEY_UP) {
    direction = 3;
  }
  if (keys & PAD_KEY_LEFT) {
    direction = 7;
  }
  if (keys & PAD_KEY_RIGHT) {
    direction = 11;
  }
  if (keys & PAD_KEY_DOWN) {
    direction = 15;
  }
  if (keys & PAD_BUTTON_B) {
    direction += 1;
  }
  if (keys & PAD_BUTTON_SELECT) {
    direction += 2;
  }
  if (keys & PAD_BUTTON_START) {
    direction = 19;
  }
  if (direction < 0) {
    return -1;
  }
  frame = (MainLoop_GetFrameCount() & 0x30) >> 4;
  return (frame * 20 + (direction + 11)) << 5;
}
