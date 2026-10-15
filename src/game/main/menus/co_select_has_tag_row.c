#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

/* Whether CO select outside a campaign shows the back-row (tag) COs: always
 * in game mode 3, in the War Room on a two-map battle while the other map's
 * unk_30 is 0 or when unk_59 is 2 or 3. */
BOOL CoSelect_HasTagRow(void) {
  if (MAP_STATE->gameMode == GAME_MODE_SURVIVAL) {
    return TRUE;
  }
  if (MAP_STATE->gameMode == GAME_MODE_WAR_ROOM) {
    if (MAP_STATE->secondMapId != 0 && OtherMap_GetUnk30() == 0) {
      return TRUE;
    }
    if (MAP_STATE->unk_59 == 2) {
      return TRUE;
    }
    if (MAP_STATE->unk_59 == 3) {
      return TRUE;
    }
  }
  return FALSE;
}
