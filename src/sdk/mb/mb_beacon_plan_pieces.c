#include "types.h"
#include "sdk/mb.h"

extern DlPlayAdvertProgress data_022ace4c;

/* Picks where the current game is cut into pieces: 9 from the icon palette on
 * when the game has an icon, else 4 from userParam. */
void MbBeacon_PlanPieces(void) {
  DlPlayGameEntry* game;

  if (data_022ace4c.state != DLPLAY_ADVERT_STEP_SPLIT) {
    return;
  }
  game = data_022ace4c.currentGame;
  if (game->noIcon == 0) {
    data_022ace4c.pieceCount = 9;
    data_022ace4c.nextPiece = game->iconPalette;
  } else {
    data_022ace4c.pieceCount = 4;
    data_022ace4c.nextPiece = game->userParam;
  }
  data_022ace4c.state = DLPLAY_ADVERT_STEP_EMIT;
}
