#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"

s8 func_020bf8b8(void);

/* Unit menu entry test: 1 hides the entry. Shown only when CO effects are on,
 * the capture check passes and the current player's front CO is Sami. */
s8 UnitMenu_SamiCaptureHidden(void) {
  if (!MAP_STATE->coEffectsOn) {
    return 1;
  }
  if (!func_020bf8b8()) {
    if ((MAP_STATE->players[MAP_STATE->currentPlayer].front.id &
         PLAYER_CO_ID_MASK) == CO_SAMI) {
      return 0;
    }
  }
  return 1;
}
