#include "types.h"
#include "sdk/mb.h"

void* func_0211ba5c(void);

/* Moves to the next game on the list, or back to the first one after the
 * last. False when the list is empty. */
BOOL MbBeacon_SelectNextGame(void) {
  DlPlayGameEntry* first = data_022ace4c.gameList;
  DlPlayGameEntry* game;

  if (first == NULL) {
    return FALSE;
  }
  game = data_022ace4c.currentGame;
  if (game == NULL || game->next == NULL) {
    game = first;
  } else {
    game = game->next;
  }
  data_022ace4c.currentGame = game;
  func_0211ba5c();
  data_022ace4c.changeCount = data_022ace4c.currentGame->changeCount;
  data_022ace4c.state = DLPLAY_ADVERT_STEP_SPLIT;
  return TRUE;
}
