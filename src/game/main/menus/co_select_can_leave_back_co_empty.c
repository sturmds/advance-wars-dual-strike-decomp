#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Whether CO select accepts an empty back-row (tag) cell. Not in a two-map
 * battle while the other map's unk_30 is 0, and not in the War Room when
 * unk_59 is 2 or 3. */
BOOL CoSelect_CanLeaveBackCoEmpty(void) {
  if (STATE->secondMapId != 0 && OtherMap_GetUnk30() == 0) {
    return FALSE;
  }
  if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
    if (STATE->unk_59 == 2) {
      return FALSE;
    }
    if (STATE->unk_59 == 3) {
      return FALSE;
    }
  }
  return TRUE;
}
