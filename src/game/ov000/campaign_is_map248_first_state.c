#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* True in campaign map 248 (campaign mission 24) on the first of the two map states. */
BOOL Campaign_IsMap248FirstState(void) {
  if (GAME->gameMode == GAME_MODE_CAMPAIGN && GAME->unk_1c4->mapId == 248 &&
      GAME == &data_02183b08) {
    return TRUE;
  }
  return FALSE;
}
