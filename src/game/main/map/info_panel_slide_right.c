#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_02166308[3]; /* panel width by unit boxes shown */

#define STATE (data_027c027c->unk_48[data_027c0284])

int InfoPanel_CountUnitBoxes(u8 x,
                             u8 y); /* unit boxes the panel shows for a tile */
void InfoPanel_FollowCursor(void);

/* One step of the info panel moving to the right side (x 164). From the left
 * (x 3 and below) it eases out past the screen edge; it then reappears at
 * x 258 and eases in. While it moves in, and once it is at rest, it describes
 * the tile under the cursor. */
void InfoPanel_SlideRight(void) {
  s16 hiddenX = -data_02166308[InfoPanel_CountUnitBoxes(STATE->infoTileX,
                                                        STATE->infoTileY)] -
                8;

  if (STATE->infoPanelX <= 3) {
    STATE->infoPanelRight = 1;
    if (STATE->infoPanelX <= hiddenX + 1) {
      STATE->infoPanelX = 258;
    } else {
      STATE->infoPanelX -= (STATE->infoPanelX - hiddenX) / 2;
    }
  }
  if (STATE->infoPanelX > 164) {
    if (STATE->infoPanelX <= 165) {
      STATE->infoPanelX = 164;
    } else {
      STATE->infoPanelX -= (STATE->infoPanelX - 164) / 2;
    }
    InfoPanel_FollowCursor();
  }
  if (STATE->infoPanelX == 164) {
    InfoPanel_FollowCursor();
  }
}
