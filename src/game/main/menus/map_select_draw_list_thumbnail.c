#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"

void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern MenuState gMenuState;
extern u8 data_027c0060[];

#define MENU ((MenuState*)&gMenuState)
#define DRAW_PIECE(px, tile)                                                  \
  if ((px) > -32 && (px) < 256) {                                             \
    Sprite_QueueAt(0, (px) & 0x1ff, y & 0xff, &data_027c0060, 0x400, (tile)); \
  }

/* Draws the thumbnail strip piece for map list `list` (wrapped to the list
 * count) at x: four 32-wide pieces, those that are on screen. `list` is
 * reused as the tile page, needed to match. */
void MapSelect_DrawListThumbnail(int x, int y, int list) {
  int px;
  int count = MENU->setup.mapListCount;
  Unk_02080440_48* state;
  int gameMode;

  do {
    if (list < 0) {
      list += count;
    }
    if (list >= count) {
      list -= count;
    }
  } while (list < 0 || list >= count);
  state = data_027c027c->unk_48[data_027c0284];
  gameMode = state->gameMode;
  if (gameMode == GAME_MODE_WAR_ROOM) {
    list = MENU->setup.lists[list].unk_64 - 9;
  } else {
    list = MENU->setup.lists[list].unk_64 - 1;
  }
  if (gameMode != GAME_MODE_WAR_ROOM) {
    if (state->twoMaps == 1) {
      y += 16;
    }
  }
  px = x - 64;
  DRAW_PIECE(px, list * 32);
  px = x - 32;
  DRAW_PIECE(px, list * 32 + 8);
  DRAW_PIECE(x, list * 32 + 16);
  x += 32;
  DRAW_PIECE(x, list * 32 + 24);
}
