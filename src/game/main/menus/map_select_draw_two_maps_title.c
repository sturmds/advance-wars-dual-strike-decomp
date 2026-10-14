#include "types.h"
#include "game/menu_state.h"
#include "game/oam_cells.h"

extern MenuState gMenuState;

void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);

/* Draws the title strip for a two-map battle: two 32-wide sprites side by
 * side, with tiles chosen by setup.firstMapId. */
void MapSelect_DrawTwoMapsTitle(int x, int y) {
  if (gMenuState.setup.firstMapId == 0) {
    Sprite_QueueAt(0, (x - 32) & 0x1ff, y & 0xff, &data_027c0060, 0x400, 0x100);
    Sprite_QueueAt(0, x & 0x1ff, y & 0xff, &data_027c0060, 0x400, 0x108);
  } else {
    Sprite_QueueAt(0, (x - 32) & 0x1ff, y & 0xff, &data_027c0060, 0x400, 0x110);
    Sprite_QueueAt(0, x & 0x1ff, y & 0xff, &data_027c0060, 0x400, 0x118);
  }
}
