#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_02166308[3]; /* panel width by unit boxes shown */

#define STATE (data_027c027c->unk_48[data_027c0284])

int InfoPanel_CountUnitBoxes(u8 x,
                             u8 y); /* unit boxes the panel shows for a tile */
void InfoPanel_FollowCursor(void);

/* One step of the info panel moving to the left side (x 3). From the right
 * (x 164 and up) it eases out past the screen edge; it then reappears at
 * x -90 and eases in. While it moves in, and once it is at rest, it describes
 * the tile under the cursor. */
void InfoPanel_SlideLeft(void) {
  s16 hiddenX = data_02166308[InfoPanel_CountUnitBoxes(STATE->infoTileX,
                                                       STATE->infoTileY)] +
                176;

  if (STATE->infoPanelX >= 164) {
    STATE->infoPanelRight = 0;
    if (STATE->infoPanelX >= hiddenX - 1) {
      STATE->infoPanelX = -90;
    } else {
      STATE->infoPanelX -= (STATE->infoPanelX - hiddenX) / 2;
    }
  }
  if (STATE->infoPanelX < 3) {
    if (STATE->infoPanelX >= 2) {
      STATE->infoPanelX = 3;
    } else {
      STATE->infoPanelX -= (STATE->infoPanelX - 3) / 2;
    }
    InfoPanel_FollowCursor();
  }
  if (STATE->infoPanelX == 3) {
    InfoPanel_FollowCursor();
  }
}
