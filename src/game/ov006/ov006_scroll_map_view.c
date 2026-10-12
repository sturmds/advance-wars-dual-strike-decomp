#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/keypad.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern int gCameraPresetIndex;
extern u8 data_021576fa[][20];
extern u8 data_021576fb[][20];
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define VIEW (GAME->unk_1c4)
/* needed to match: some reads load the index before the state array. */
#define GAME_IDX_FIRST                                                    \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))
#define VIEW_AGAIN (GAME_IDX_FIRST->unk_1c4)
#define STATE_50 (data_027c027c->unk_50[data_027c0284])

/* MapView_ScrollStep for the Design Room. The view follows the tile in
 * viewX, viewY: while X is held that tile tracks the map cursor
 * (cursorX, cursorY); otherwise a held direction puts the cursor back on it.
 * The scrolling itself works as in MapView_ScrollStep. */
void Ov006_ScrollMapView(int step) {
  int layout;
  int cursorX;
  int cursorY;
  int dist;
  u8 viewW;
  u8 viewH;

  layout = gCameraPresetIndex;
  viewW = data_021576fa[layout][0];
  viewH = data_021576fb[layout][0];
  if (gKeypad->held & PAD_BUTTON_X) {
    cursorX = GAME->cursorX;
    cursorY = GAME->cursorY;
    data_ov006_0236d640->viewX = cursorX;
    data_ov006_0236d640->viewY = cursorY;
  } else {
    cursorX = data_ov006_0236d640->viewX;
    cursorY = data_ov006_0236d640->viewY;
    if (gKeypad->held &
        (PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN)) {
      GAME->cursorX = cursorX;
      GAME->cursorY = cursorY;
    }
  }

  /* needed to match: x reuses cursorX for the distance, y has its own. */
  if ((VIEW_AGAIN->scrollFlags & 3) == 0) {
    cursorX -= VIEW->scrollX >> 4;
    if (cursorX < 2) {
      VIEW->scrollFlags |= 2;
    }
    if (cursorX >= viewW - 2) {
      VIEW->scrollFlags |= 1;
    }
  }
  if ((VIEW_AGAIN->scrollFlags & 12) == 0) {
    dist = cursorY - (VIEW->scrollY >> 4);
    if (dist < 2) {
      VIEW->scrollFlags |= 4;
    }
    if (dist >= viewH - 2) {
      VIEW->scrollFlags |= 8;
    }
  }

  if (VIEW->scrollFlags & 1) {
    if (viewW == VIEW->mapWidth - (VIEW->scrollX >> 4)) {
      VIEW->scrollFlags ^= 1;
    } else {
      VIEW->scrollX += step;
      if ((VIEW_AGAIN->scrollX & 15) == 0) {
        VIEW_AGAIN->scrollFlags ^= 1;
      }
    }
  }
  if (VIEW->scrollFlags & 2) {
    if (VIEW->scrollX == 0) {
      VIEW->scrollFlags ^= 2;
    } else {
      VIEW->scrollX = VIEW->scrollX - step;
      if ((VIEW_AGAIN->scrollX & 15) == 0) {
        VIEW_AGAIN->scrollFlags ^= 2;
      }
    }
  }
  if (VIEW->scrollFlags & 4) {
    if (VIEW->scrollY == 0) {
      VIEW->scrollFlags ^= 4;
    } else {
      VIEW->scrollY = VIEW->scrollY - step;
      if ((VIEW_AGAIN->scrollY & 15) == 0) {
        VIEW_AGAIN->scrollFlags ^= 4;
      }
    }
  }
  if (VIEW->scrollFlags & 8) {
    if (viewH == VIEW->mapHeight - (VIEW->scrollY >> 4)) {
      VIEW->scrollFlags ^= 8;
    } else {
      VIEW->scrollY += step;
      if ((VIEW_AGAIN->scrollY & 15) == 0) {
        VIEW_AGAIN->scrollFlags ^= 8;
      }
    }
  }

  if (STATE_50->callback == NULL) {
    return;
  }
  STATE_50->callback();
}
