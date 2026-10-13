#include "types.h"
#include "sdk/mb.h"

/* Adds a game to the end of the beacon's list. */
void MbBeacon_AppendGame(DlPlayGameEntry* game) {
  DlPlayGameEntry* last = data_022ace4c.gameList;
  DlPlayGameEntry* next;

  if (last == NULL) {
    data_022ace4c.gameList = game;
  } else {
    next = last->next;
    while (next != NULL) {
      last = next;
      next = next->next;
    }
    last->next = game;
  }
  game->next = NULL;
}
