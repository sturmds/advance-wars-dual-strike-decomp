#include "types.h"
#include "game/Unk_02080440.h"

BOOL CanUseTagPower(int player);

/* Battle menu entry test: 1 hides the entry. Same shape as the Super and CO
 * power entries beside it; this one is for the tag power (handler
 * PowerMenu_ActivateTag). */
s8 PowerMenu_TagHidden(void) {
  if (MAP_STATE->unk_5c == 0) {
    return 1;
  }
  return CanUseTagPower(MAP_STATE->currentPlayer) == 0;
}
