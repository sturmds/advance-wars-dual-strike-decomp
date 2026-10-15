#include "types.h"
#include "game/co_select_screen.h"
#include "game/Unk_02088d04.h"
#include "game/math.h"
#include "game/screen_other_map.h"
#include "game/win_quote.h"

#define PICTURE_OUT 144 /* slide offset with the picture off screen */
#define SLIDE_FRAMES 8
#define OUT_WAIT_FRAMES 4
#define PICTURE_TILES 0x1b0 /* 32-byte tiles per row's picture */

/* CO in grid cell (column 1-4, row 0-1). Needed to match: coGrid read as rows of 2. */
#define GRID_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])

void Mem_Zero32(u32 value, void* dest, u32 size);
void Co_LoadGraphicsAndPalette(int co, int tile, int row);

/* When the cursor moves to another CO, the row's picture slides out, stays
 * out 4 frames, then the new CO's picture loads and slides back in. At most
 * one row loads a picture per frame. Each row has its own tiles and palette. */
void CoSelectScreen_SlidePictures(CoSelectPictureTask* task) {
  BOOL loaded;
  Unk_027c0280* savedGfx;
  int row;

  savedGfx = data_027c0280;
  loaded = FALSE;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  for (row = 0; row < data_0229c73c->rowCount; row++) {
    if (task->co[row] != GRID_CO(data_0229c73c->cursorColumn, row)) {
      task->co[row] = GRID_CO(data_0229c73c->cursorColumn, row);
      if (task->waitFrames[row] == 0) {
        if (task->co[row] == task->shownCo[row]) {
          if (task->slidingIn[row] == 0) {
            task->slideFrom[row] = task->slideOffset[row];
            task->frame[row] = 0;
            task->slidingIn[row] = 1;
          }
        } else if (task->slidingIn[row] == 1) {
          task->slideFrom[row] = task->slideOffset[row];
          task->frame[row] = 0;
          task->slidingIn[row] = 0;
        }
      }
      task->waitFrames[row] = 0;
    }
    if (task->slidingIn[row] == 0) {
      task->slideOffset[row] =
          task->slideFrom[row] +
          Math_Ease(1, 0, PICTURE_OUT, task->frame[row], SLIDE_FRAMES);
      if (task->slideOffset[row] > PICTURE_OUT) {
        task->slideOffset[row] = PICTURE_OUT;
      }
    } else {
      task->slideOffset[row] =
          Math_Ease(4, task->slideFrom[row], 0, task->frame[row], SLIDE_FRAMES);
    }
    if (row == 0) {
      Sprite_QueueCoPortrait((56 - task->slideOffset[row]) & 0x1ff, 160,
                             (row << 12) | 0x800, row * PICTURE_TILES, row + 5,
                             TRUE);
    } else {
      Sprite_QueueCoPortrait((256 - (56 - task->slideOffset[row])) & 0x1ff, 160,
                             (row << 12) | 0x800, row * PICTURE_TILES, row + 5,
                             FALSE);
    }
    if (task->frame[row] == SLIDE_FRAMES) {
      if (task->slidingIn[row] == 0) {
        if (task->waitFrames[row] < OUT_WAIT_FRAMES) {
          task->waitFrames[row]++;
        } else if (!loaded) {
          task->shownCo[row] = task->co[row];
          if (GRID_CO(data_0229c73c->cursorColumn, row) == 0) {
            Mem_Zero32(0,
                       (u8(*)[32])data_027c0280->objVram + row * PICTURE_TILES,
                       PICTURE_TILES * 32);
          } else {
            Co_LoadGraphicsAndPalette(task->co[row], row * PICTURE_TILES, row);
          }
          loaded = TRUE;
          task->slideFrom[row] = task->slideOffset[row];
          task->slidingIn[row] = 1;
          task->frame[row] = 0;
          task->waitFrames[row] = 0;
        }
      }
    } else {
      task->frame[row]++;
    }
  }
  Screen_SetCurrentGfx(savedGfx);
}
