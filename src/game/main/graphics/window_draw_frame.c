#include "types.h"

void Gfx_RequestLayerCopy(int layer);
void Window_DrawFrameTopRow(u16* row, int x, s16 width, int palette);
void Window_DrawFrameMiddleRow(u16* row, int x, s16 width, u16 rowKind,
                               int palette);
void Window_DrawFrameBottomRow(u16* row, int x, s16 width, int palette);

/* Writes a window frame of width x height tiles at tile (x, y) of a
 * 32-tile-wide map: a top row, height - 2 middle rows and a bottom row, each
 * with palette bank palette, then asks for layer 1 to be copied to VRAM. */
void Window_DrawFrame(int x, int y, int width, int height, u16* map,
                      int palette) {
  u16* row = map + x + y * 32;
  u16 i;
  int middleRows;

  Window_DrawFrameTopRow(row, x, width, palette);
  middleRows = height - 2;
  row += 32;
  for (i = 0; i < middleRows; i++) {
    Window_DrawFrameMiddleRow(row, x, width, 1, palette);
    row += 32;
  }
  Window_DrawFrameBottomRow(row, x, width, palette);
  Gfx_RequestLayerCopy(1);
}
