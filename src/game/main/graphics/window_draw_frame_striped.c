#include "types.h"

void Gfx_RequestLayerCopy(int layer);
void Window_DrawFrameTopRow(u16* row, int x, s16 width, int palette);
void Window_DrawFrameMiddleRow(u16* row, int x, s16 width, s16 rowKind,
                               int palette);
void Window_DrawFrameBottomRow(u16* row, int x, s16 width, int palette);

/* Like Window_DrawFrame, but the middle rows alternate between row kinds 1
 * and 0 (Window_DrawFrameMiddleRow picks the tiles by kind). */
void Window_DrawFrameStriped(int x, int y, int width, int height, u16* map,
                             int palette) {
  u16* row = map + x + y * 32;
  u16 i;
  u16 rowKind;
  int middleRows;

  Window_DrawFrameTopRow(row, x, width, palette);
  middleRows = height - 2;
  row += 32;
  rowKind = 1;
  for (i = 0; i < middleRows; i++) {
    Window_DrawFrameMiddleRow(row, x, width, rowKind, palette);
    row += 32;
    rowKind = 1 - rowKind;
  }
  Window_DrawFrameBottomRow(row, x, width, palette);
  Gfx_RequestLayerCopy(1);
}
