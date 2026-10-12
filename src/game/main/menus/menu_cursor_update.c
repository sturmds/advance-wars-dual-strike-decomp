#include "types.h"
#include "game/menu_button.h"
#include "game/touch.h"

int MainLoop_GetFrameCount(void);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);
int MenuButton_TestFlags(MenuButton* entry, int mask);

extern u8 data_02158f3c[];
extern u8 gMenuCursorShake[16];

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define EASE(cur, target) \
  (ABS((target) - (cur)) <= 1 ? (target) : (cur) + (((target) - (cur)) >> 1))

/* Runs the menu cursor each frame: each corner moves half the way to the
 * matching corner of the selected button (or jumps there while
 * the screen is touched), then the four corner sprites are drawn if the
 * button has flag 1. On frames where MainLoop_GetFrameCount() & 0x30 is 0 the
 * corners shake by offsets from gMenuCursorShake. */
void MenuCursor_Update(void) {
  MenuButton* button;
  int i;
  int mode;
  int left;
  int top;
  int right;
  int bottom;
  int tile;

  mode = 0;
  MenuButton_Select(gMenuCursor->selectedId);
  if (!MenuCursor_HasSelection()) {
    return;
  }
  button = gMenuCursor->selected;
  left = button->x;
  top = button->y;
  right = left + button->width;
  bottom = top + button->height;
  if (gTouch.phase == TOUCH_RELEASED) {
    gMenuCursor->corners[0][0] = EASE(gMenuCursor->corners[0][0], left);
    gMenuCursor->corners[0][1] = EASE(gMenuCursor->corners[0][1], top);
    gMenuCursor->corners[1][0] = EASE(gMenuCursor->corners[1][0], right);
    gMenuCursor->corners[1][1] = EASE(gMenuCursor->corners[1][1], top);
    gMenuCursor->corners[2][0] = EASE(gMenuCursor->corners[2][0], left);
    gMenuCursor->corners[2][1] = EASE(gMenuCursor->corners[2][1], bottom);
    gMenuCursor->corners[3][0] = EASE(gMenuCursor->corners[3][0], right);
    gMenuCursor->corners[3][1] = EASE(gMenuCursor->corners[3][1], bottom);
  } else {
    /* needed to match: the original sign-extends each value once */
    s16 l = left;
    s16 t = top;
    s16 r = right;
    s16 b = bottom;
    gMenuCursor->corners[0][0] = l;
    gMenuCursor->corners[0][1] = t;
    gMenuCursor->corners[1][0] = r;
    gMenuCursor->corners[1][1] = t;
    gMenuCursor->corners[2][0] = l;
    gMenuCursor->corners[2][1] = b;
    gMenuCursor->corners[3][0] = r;
    gMenuCursor->corners[3][1] = b;
  }

  button = MenuButton_Find(gMenuCursor->selectedId);
  if (MenuButton_TestFlags(button, 1) == 0) {
    return;
  }
  if (MenuButton_TestFlags(button, 0x800) != 0) {
    mode = 0x400;
  }
  for (i = 0; i < 4; i++) {
    if (MainLoop_GetFrameCount() & 0x30) {
      tile = gMenuCursor->tile + i * 2;
      Oam_PutSprite(gMenuCursor->corners[i][0] & 0x1ff,
                    mode | (gMenuCursor->corners[i][1] & 0xff), data_02158f3c,
                    tile | (gMenuCursor->palette << 12));
    } else {
      /* left and top now hold the shake offsets; separate locals
             * get worse register allocation */
      left = gMenuCursorShake[MainLoop_GetFrameCount() & 15];
      top = gMenuCursorShake[MainLoop_GetFrameCount() & 15];
      if (i == 1 || i == 3) {
        left = -left;
      }
      if (i == 2 || i == 3) {
        top = -top;
      }
      tile = gMenuCursor->tile + i * 2;
      Oam_PutSprite((left + gMenuCursor->corners[i][0]) & 0x1ff,
                    mode | ((top + gMenuCursor->corners[i][1]) & 0xff),
                    data_02158f3c, tile | (gMenuCursor->palette << 12));
    }
  }
}
