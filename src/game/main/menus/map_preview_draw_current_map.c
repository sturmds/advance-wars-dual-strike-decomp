#include "types.h"
#include "game/map_data.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;

void func_02020848(void* dst);
void func_02020854(void* dst);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Copies the map being played (size and the cell code of every tile) into
 * data_0229eadc, draws its preview into dst in both halves, and scrolls
 * layer 0 so the preview, 4 pixels per tile, sits in the middle of the
 * 256x192 screen. */
void MapPreview_DrawCurrentMap(void* dst) {
  int col;
  int row;

  data_0229eadc->width = GAME->unk_1c4->mapWidth;
  data_0229eadc->height = GAME->unk_1c4->mapHeight;
  for (row = 0; row < GAME->unk_1c4->mapHeight; row++) {
    for (col = 0; col < GAME->unk_1c4->mapWidth; col++) {
      data_0229eadc->tileCodes[col + GAME->mapTiles->rowStart[row]] =
          GAME->mapTiles->mapCells[col + GAME->mapTiles->rowStart[row]];
    }
  }
  func_02020854(dst);
  func_02020848(dst);
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] =
      -(256 - data_0229eadc->width * 4) >> 1;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] =
      -(192 - data_0229eadc->height * 4) >> 1;
}
