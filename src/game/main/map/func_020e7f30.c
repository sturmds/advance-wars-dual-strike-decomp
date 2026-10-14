#include "types.h"
#include "game/co_data.h"
#include "game/map_header.h"

BOOL Campaign_IsHardMode(void);

/* TRUE if Campaign_IsHardMode says so, or if the map lets any player pick a CO. */
BOOL func_020e7f30(int mapId) {
  int player;
  int slot;

  if (Campaign_IsHardMode() == TRUE) {
    return TRUE;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    for (slot = 0; slot < PLAYER_CO_COUNT; slot++) {
      if (Ov000_gMapHeaders[mapId].co[player][slot] == CO_PLAYER_PICKS) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
