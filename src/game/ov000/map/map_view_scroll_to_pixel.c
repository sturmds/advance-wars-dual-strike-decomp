#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern int gCameraPresetIndex;
extern u8 data_021576fa[][20];
extern u8 data_021576fb[][20];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define STATE_50 (data_027c027c->unk_50[data_027c0284])
#define NO_SCROLL 10000

/* Scrolls the map view so the pixel position (x, y) is on screen. The view
 * moves only when the position is within 2 tiles of the left or top edge, or
 * 3 tiles of the right or bottom edge (the 1/16-tile units of the scroll
 * fields), and stays inside the map. Runs the state's callback after a
 * scroll. Local order and types are needed to match. */
void MapView_ScrollToPixel(int x, int y) {
  u8 viewW;
  u8 viewH;
  s16 newX;
  s16 newY;
  Unk_02080440_1c4* view;
  s16 scrollX;
  s16 scrollY;

  viewW = data_021576fa[gCameraPresetIndex][0];
  viewH = data_021576fb[gCameraPresetIndex][0];

  newX = NO_SCROLL;
  view = GAME->unk_1c4;
  newY = NO_SCROLL;
  scrollX = view->scrollX;
  if (x - scrollX < 32) {
    newX = x - 32;
  }
  if (x - scrollX > (viewW - 3) * 16) {
    newX = x - (viewW - 3) * 16;
  }
  scrollY = view->scrollY;
  if (y - scrollY < 32) {
    newY = y - 32;
  }
  if (y - scrollY > (viewH - 3) * 16) {
    newY = y - (viewH - 3) * 16;
  }
  if (newX == NO_SCROLL && newY == NO_SCROLL) {
    return;
  }
  if (newX == NO_SCROLL) {
    newX = scrollX;
  } else if (newX < 0) {
    newX = 0;
  } else if (newX > (view->mapWidth - viewW) * 16) {
    newX = (view->mapWidth - viewW) * 16;
  }
  if (newY == NO_SCROLL) {
    newY = scrollY;
  } else if (newY < 0) {
    newY = 0;
  } else if (newY > (view->mapHeight - viewH) * 16) {
    newY = (view->mapHeight - viewH) * 16;
  }
  view->scrollX = newX;
  GAME->unk_1c4->scrollY = newY;
  if (STATE_50->callback == NULL) {
    return;
  }
  STATE_50->callback();
}
