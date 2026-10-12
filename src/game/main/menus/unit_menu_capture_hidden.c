#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s8 func_020bf8b8(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test: 1 hides the entry. The plain Capture entry; with CO
 * effects on, Sami's front CO gets UnitMenu_SamiCaptureHidden instead. */
s8 UnitMenu_CaptureHidden(void) {
  if (!func_020bf8b8()) {
    if (!STATE->coEffectsOn || (STATE->players[STATE->currentPlayer].front.id &
                                PLAYER_CO_ID_MASK) != CO_SAMI) {
      return 0;
    }
  }
  return 1;
}
