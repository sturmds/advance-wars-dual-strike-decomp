#include "types.h"
#include "game/Unk_02080440.h"

extern u16 data_02166308[]; /* panel width by unit boxes shown */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

int InfoPanel_CountUnitBoxes(u8 x,
                             u8 y); /* unit boxes the panel shows for a tile */
void InfoPanel_FollowCursor(void);

/* One step of the info panel moving to the left side (x 3). From the right
 * (x 164 and up) it eases out past the screen edge; it then reappears at
 * x -90 and eases in. While it moves in, and once it is at rest, it describes
 * the tile under the cursor. */
void InfoPanel_SlideLeft(void) {
  s16 hiddenX = data_02166308[InfoPanel_CountUnitBoxes(MAP_STATE->infoTileX,
                                                       MAP_STATE->infoTileY)] +
                176;

  if (MAP_STATE->infoPanelX >= 164) {
    MAP_STATE->infoPanelRight = 0;
    if (MAP_STATE->infoPanelX >= hiddenX - 1) {
      MAP_STATE->infoPanelX = -90;
    } else {
      MAP_STATE->infoPanelX -= (MAP_STATE->infoPanelX - hiddenX) / 2;
    }
  }
  if (MAP_STATE->infoPanelX < 3) {
    if (MAP_STATE->infoPanelX >= 2) {
      MAP_STATE->infoPanelX = 3;
    } else {
      MAP_STATE->infoPanelX -= (MAP_STATE->infoPanelX - 3) / 2;
    }
    InfoPanel_FollowCursor();
  }
  if (MAP_STATE->infoPanelX == 3) {
    InfoPanel_FollowCursor();
  }
}
