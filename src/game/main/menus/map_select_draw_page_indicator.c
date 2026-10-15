#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/oam_cells.h"
#include "game/math.h"
#include "game/map_select.h"

void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);

#define MENU ((MenuState*)&gMenuState)

/* Draws the page indicator of a map list: the digit of the current page
 * (1-based), a separator, and the digit of the page count. */
void MapSelect_DrawPageIndicator(int x, int y, int list) {
  int columns;
  int rows;
  int pages;

  if (x < -24) {
    return;
  }
  if (x > 256) {
    return;
  }
  Sprite_QueueAt(0, x & 0x1ff, y & 0xff, &data_027c0028, 0x1000,
                 (data_0229c768.unk_0d0[MapSelect_GetTab()]
                                       [MENU->setup.lists[list].unk_64] +
                  1) * 2 +
                     0x180);
  Sprite_QueueAt(0, (x + 8) & 0x1ff, y & 0xff, &data_027c0028, 0x1000, 0x194);
  columns = MAP_STATE->gameMode == GAME_MODE_WAR_ROOM
                ? 7
                : (MAP_STATE->twoMaps == 0 ? 9 : 8);
  rows = MAP_STATE->gameMode == GAME_MODE_WAR_ROOM ? 1 : 3;
  pages = HwDiv32(MENU->setup.lists[data_0229c768.listOfTab[MapSelect_GetTab()]]
                          .mapCount -
                      1,
                  rows * columns) +
          1;
  Sprite_QueueAt(0, (x + 16) & 0x1ff, y & 0xff, &data_027c0028, 0x1000,
                 pages * 2 + 0x180);
}
