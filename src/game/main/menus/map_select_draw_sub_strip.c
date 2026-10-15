#include "types.h"
#include "game/menu_state.h"
#include "game/oam_cells.h"

int MapSelect_GetTab(void);
void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);

extern MapListTables data_0229c768;

#define MENU ((MenuState*)&gMenuState)
#define LIST() (MENU->setup.lists[data_0229c768.listOfTab[MapSelect_GetTab()]])
#define DRAW_PIECE(px, tile)                                                  \
  if ((px) > -32 && (px) < 256) {                                             \
    Sprite_QueueAt(0, (px) & 0x1ff, y & 0xff, &data_027c0060, 0x400, (tile)); \
  }

/* Draws page `entry` (wrapped to 0-3) of the sub strip at x: four 32-wide
 * pieces, those that are on screen. The tiles are offset by 4 pages, and by
 * 4 more when the current list is entry 12. */
void MapSelect_DrawSubStrip(int x, int y, int entry) {
  int px;

  do {
    if (entry < 0) {
      entry += 4;
    }
    if (entry >= 4) {
      entry -= 4;
    }
  } while (entry < 0 || entry >= 4);
  entry += 4;
  if (LIST().unk_64 == 12) {
    entry += 4;
  }
  px = x - 64;
  DRAW_PIECE(px, entry * 32);
  px = x - 32;
  DRAW_PIECE(px, entry * 32 + 8);
  DRAW_PIECE(x, entry * 32 + 16);
  x += 32;
  DRAW_PIECE(x, entry * 32 + 24);
}
