#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"

s8 func_020bf8b8(void);

/* Unit menu entry test: 1 hides the entry. The plain Capture entry; with CO
 * effects on, Sami's front CO gets UnitMenu_SamiCaptureHidden instead. */
s8 UnitMenu_CaptureHidden(void) {
  if (!func_020bf8b8()) {
    if (!MAP_STATE->coEffectsOn ||
        (MAP_STATE->players[MAP_STATE->currentPlayer].front.id &
         PLAYER_CO_ID_MASK) != CO_SAMI) {
      return 0;
    }
  }
  return 1;
}
