#include "types.h"
#include "game/map_data.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void func_02020848(void* dst);
void func_02020854(void* dst);

/* Copies the map being played (size and the cell code of every tile) into
 * data_0229eadc, draws its preview into dst in both halves, and scrolls
 * layer 0 so the preview, 4 pixels per tile, sits in the middle of the
 * 256x192 screen. */
void MapPreview_DrawCurrentMap(void* dst) {
  int col;
  int row;

  data_0229eadc->width = MAP_STATE->unk_1c4->mapWidth;
  data_0229eadc->height = MAP_STATE->unk_1c4->mapHeight;
  for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    for (col = 0; col < MAP_STATE->unk_1c4->mapWidth; col++) {
      data_0229eadc->tileCodes[col + MAP_STATE->mapTiles->rowStart[row]] =
          MAP_STATE->mapTiles
              ->mapCells[col + MAP_STATE->mapTiles->rowStart[row]];
    }
  }
  func_02020854(dst);
  func_02020848(dst);
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] =
      -(256 - data_0229eadc->width * 4) >> 1;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] =
      -(192 - data_0229eadc->height * 4) >> 1;
}
