#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440_48 data_02183b08;

/* True in campaign map 248 (campaign mission 24) on the first of the two map states. */
BOOL Campaign_IsMap248FirstState(void) {
  if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN &&
      MAP_STATE->unk_1c4->mapId == 248 && MAP_STATE == &data_02183b08) {
    return TRUE;
  }
  return FALSE;
}
