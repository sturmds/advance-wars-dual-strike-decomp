#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Whether CO select outside a campaign shows the back-row (tag) COs: always
 * in game mode 3, in the War Room on a two-map battle while the other map's
 * unk_30 is 0 or when unk_59 is 2 or 3. */
BOOL CoSelect_HasTagRow(void) {
  if (STATE->gameMode == GAME_MODE_SURVIVAL) {
    return TRUE;
  }
  if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
    if (STATE->secondMapId != 0 && OtherMap_GetUnk30() == 0) {
      return TRUE;
    }
    if (STATE->unk_59 == 2) {
      return TRUE;
    }
    if (STATE->unk_59 == 3) {
      return TRUE;
    }
  }
  return FALSE;
}
