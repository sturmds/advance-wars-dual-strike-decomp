#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/co_select_screen.h"
#include "game/oam_cells.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

int Campaign_IsHardMode(void);
void Gfx3d_InitScene(void);
void Gx_DrawTexturedQuadFormat(int x, int y, int z, int sizeS, int sizeT,
                               int texFmt, u32 texAddr, u32 palAddr, int flipX,
                               int flipY);
int HwDiv32(int a, int b);
int HwMod32(int a, int b);
int MainLoop_GetFrameCount(void);
void MenuCursor_DrawAt(int x, int y, int width, int height);
void Screen_SetCurrentGfx(void* state);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Sprite_QueueDefault(int x, int y, int attr, int list);
int func_02028644(void);
void Sprite_QueueCoSlotFrame(int a0, u16 x, u16 y, int a3);
int CoSelectScreen_RowTileY(u8 row);
int CoSelectScreen_ColumnTileX(u8 column);
BOOL CoSelectScreen_IsCoInOtherCell(u8 listCo);
int CoSelectScreen_ListSlotOfCo(u8 listCo);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* A campaign map on normal mode lists only the map's own COs, in 5 rows
 * of 24 pixels; otherwise the list has 7 rows of 16 pixels. Needed to
 * match: the column count tests the mode though both sides are 4, and the
 * row count turns the test into 0 or 1 first. SLOT_X is unsigned so the
 * offsets added to it stay separate adds. */
#define MAP_CO_LIST \
  (STATE->gameMode == GAME_MODE_CAMPAIGN && !Campaign_IsHardMode())
#define LIST_COLUMNS (MAP_CO_LIST ? 4 : 4)
#define LIST_ROWS ((MAP_CO_LIST ? 1 : 0) ? 5 : 7)
#define SLOT_Y(i) \
  (MAP_CO_LIST ? HwDiv32(i, 4) * 24 + 72 : HwDiv32(i, 4) * 16 + 72)
#define SLOT_X(i) ((u32)(HwMod32(i, LIST_COLUMNS) * 56 + 8))

/* CO in grid cell (column 1-4, row 0-1). Needed to match: coGrid read as rows of 2. */
#define GRID_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])
#define CELL(column, row) (((column) - 1) * 2 + (row))
/* Needed to match: a shift, not * 2. */
#define CURSOR_CELL \
  (((data_0229c73c->cursorColumn - 1) << 1) + data_0229c73c->cursorRow)

/* Draws the CO select screen each frame. For the CO list: a frame per
 * slot that is not 0xff, a frame at slot unk_37, and the CO pictures in 3D
 * (two quads per slot holding a CO). For the grid: the cursor box while
 * the list is open, a small icon per filled cell, and a marker per cell
 * beside its CO's list slot (16 pixels in instead of 32 for the cursor
 * cell when another cell holds the same CO). Markers of empty cells stack
 * leftwards from slot unk_37, row 0 cells first. While the list is open,
 * the cursor cell's marker blinks every 32 frames. */
void CoSelectScreen_Draw(void) {
  Unk_027c0280* savedGfx;
  u8* listCo;
  u32 co; /* needed to match: u32, and reused for the CO's list slot */
  u8 emptyCells[8];
  CoSelectScreen* screen;
  long i; /* needed to match: int gets other registers */
  int column;
  u8 emptyCount;
  int row; /* needed to match: also counts the empty cells */
  int y;
  int x; /* needed to match: also the shift counter */

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  listCo = data_0229c73c->coList;
  for (i = 0; i < LIST_ROWS * LIST_COLUMNS; i++, listCo++) {
    if (*listCo != 0xff) {
      y = SLOT_Y(i);
      x = SLOT_X(i);
      Sprite_QueueCoSlotFrame(2, x & 0x1ff, y & 0xff, 0);
    }
  }
  y = SLOT_Y(data_0229c73c->unk_37);
  x = SLOT_X(data_0229c73c->unk_37);
  Sprite_QueueAt(3, x & 0x1ff, y & 0xff, &data_027c00b8, 0, 0);

  screen = data_0229c73c;
  if (screen->coListOpen == 1) {
    x = CoSelectScreen_ColumnTileX(screen->cursorColumn);
    MenuCursor_DrawAt((x + 1) * 8,
                      (CoSelectScreen_RowTileY(screen->cursorRow) + 2) * 8, 48,
                      16);
  }

  emptyCount = 0;
  for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
    for (row = 0; row < data_0229c73c->rowCount; row++) {
      co = GRID_CO(column, row);
      if (co == 0) {
        if (row == 0) {
          for (x = emptyCount; x > 0; x--) {
            emptyCells[x] = emptyCells[x - 1];
          }
          emptyCells[0] = CELL(column, row);
        } else {
          emptyCells[emptyCount] = CELL(column, row);
        }
        emptyCount++;
        continue;
      }
      co = CoSelectScreen_ListSlotOfCo(co);
      Sprite_QueueDefault((CoSelectScreen_ColumnTileX(column) + 1) * 8 & 0x1ff,
                          (CoSelectScreen_RowTileY(row) + 2) * 8 & 0xff,
                          (CELL(column, row) * 12 + 44) | 0x1000, 2);
      if (data_0229c73c->coListOpen == 1 && (MainLoop_GetFrameCount() & 32) &&
          CURSOR_CELL == CELL(column, row)) {
        continue;
      }
      if (CURSOR_CELL == CELL(column, row) &&
          CoSelectScreen_IsCoInOtherCell(GRID_CO(
              data_0229c73c->cursorColumn, data_0229c73c->cursorRow)) == 1) {
        y = SLOT_Y(co);
        x = SLOT_X(co) + 16;
        Sprite_QueueAt(1, x & 0x1ff, y & 0xff, &data_027c0040,
                       CELL(column, row) * 4 + 12, 0);
      } else {
        y = SLOT_Y(co);
        x = SLOT_X(co) + 32;
        Sprite_QueueAt(1, x & 0x1ff, y & 0xff, &data_027c0040,
                       CELL(column, row) * 4 + 12, 0);
      }
    }
  }

  for (row = 0; row < emptyCount; row++) {
    if (data_0229c73c->coListOpen == 1 && (MainLoop_GetFrameCount() & 32) &&
        CURSOR_CELL == emptyCells[emptyCount - row - 1]) {
      continue;
    }
    if (emptyCount == 4) {
      y = SLOT_Y(data_0229c73c->unk_37);
      x = SLOT_X(data_0229c73c->unk_37) + 32 - row * 12;
      Sprite_QueueAt(1, x & 0x1ff, y & 0xff, &data_027c0040,
                     emptyCells[emptyCount - row - 1] * 4 + 12, 0);
    } else {
      y = SLOT_Y(data_0229c73c->unk_37);
      x = SLOT_X(data_0229c73c->unk_37) + 32 - row * 16;
      Sprite_QueueAt(1, x & 0x1ff, y & 0xff, &data_027c0040,
                     emptyCells[emptyCount - row - 1] * 4 + 12, 0);
    }
  }

  Gfx3d_InitScene();
  listCo = data_0229c73c->coList;
  for (i = 0; i < LIST_ROWS * LIST_COLUMNS; i++, listCo++) {
    if (*listCo != 0xff && *listCo != 0) {
      Gx_DrawTexturedQuadFormat(SLOT_X(i), SLOT_Y(i), 0, 2, 1, 3, i * 384,
                                i * 32, 0, 0);
      y = SLOT_Y(i);
      x = SLOT_X(i);
      Gx_DrawTexturedQuadFormat(x + 32, y, 0, 1, 1, 3, (i * 12 + 8) * 32,
                                i * 32, 0, 0);
    }
  }
  func_02028644();
  Screen_SetCurrentGfx(savedGfx);
}
