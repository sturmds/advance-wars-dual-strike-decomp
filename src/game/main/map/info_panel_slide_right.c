#include "types.h"
#include "game/Unk_02080440.h"

extern u16 data_02166308[]; /* panel width by unit boxes shown */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

int InfoPanel_CountUnitBoxes(u8 x,
                             u8 y); /* unit boxes the panel shows for a tile */
void InfoPanel_FollowCursor(void);

/* One step of the info panel moving to the right side (x 164). From the left
 * (x 3 and below) it eases out past the screen edge; it then reappears at
 * x 258 and eases in. While it moves in, and once it is at rest, it describes
 * the tile under the cursor. */
void InfoPanel_SlideRight(void) {
  s16 hiddenX = -data_02166308[InfoPanel_CountUnitBoxes(MAP_STATE->infoTileX,
                                                        MAP_STATE->infoTileY)] -
                8;

  if (MAP_STATE->infoPanelX <= 3) {
    MAP_STATE->infoPanelRight = 1;
    if (MAP_STATE->infoPanelX <= hiddenX + 1) {
      MAP_STATE->infoPanelX = 258;
    } else {
      MAP_STATE->infoPanelX -= (MAP_STATE->infoPanelX - hiddenX) / 2;
    }
  }
  if (MAP_STATE->infoPanelX > 164) {
    if (MAP_STATE->infoPanelX <= 165) {
      MAP_STATE->infoPanelX = 164;
    } else {
      MAP_STATE->infoPanelX -= (MAP_STATE->infoPanelX - 164) / 2;
    }
    InfoPanel_FollowCursor();
  }
  if (MAP_STATE->infoPanelX == 164) {
    InfoPanel_FollowCursor();
  }
}
