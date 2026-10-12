#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern int gCameraPresetIndex;
extern u8 data_021576fa[][20];
extern u8 data_021576fb[][20];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define VIEW (GAME->unk_1c4)
/* needed to match: after a scroll store the original reloads the index first. */
#define GAME_IDX_FIRST                                                    \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))
#define VIEW_AGAIN (GAME_IDX_FIRST->unk_1c4)
#define STATE_50 (data_027c027c->unk_50[data_027c0284])

/* One frame of the map view scrolling toward the cursor tile (cursorX, cursorY).
 * The view's scrollFlags hold the directions in progress: 1 right, 2 left,
 * 4 up, 8 down. A direction starts when the cursor is within 2 tiles of that
 * edge of the view, and stops at a map edge or when the scroll reaches a whole
 * tile. step is added to or taken from the scroll (in 1/16 tiles). Runs the
 * state's callback at the end. */
void MapView_ScrollStep(int step) {
  int viewW;
  int viewH;
  int layout;
  int dist;

  layout = gCameraPresetIndex;
  viewW = data_021576fa[layout][0];
  viewH = data_021576fb[layout][0];

  if ((VIEW->scrollFlags & 3) == 0) {
    dist = GAME->cursorX - (VIEW->scrollX >> 4);
    if (dist < 2) {
      VIEW->scrollFlags |= 2;
    }
    if (dist >= viewW - 2) {
      VIEW->scrollFlags |= 1;
    }
  }
  if ((VIEW->scrollFlags & 12) == 0) {
    dist = GAME->cursorY - (VIEW->scrollY >> 4);
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
