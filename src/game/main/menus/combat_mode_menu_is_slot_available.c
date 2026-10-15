#include "types.h"
#include "game/unlocks.h"

/* Slots 0-2 of the Combat mode menu always exist; slots 3-6 need game
 * flags 50-53. */
BOOL CombatModeMenu_IsSlotAvailable(u8 slot) {
  switch (slot) {
    case 0:
    case 1:
    case 2:
      return TRUE;
    case 3:
      if (GameFlag_IsSet(50)) {
        return TRUE;
      }
      break;
    case 4:
      if (GameFlag_IsSet(51)) {
        return TRUE;
      }
      break;
    case 5:
      if (GameFlag_IsSet(52)) {
        return TRUE;
      }
      break;
    case 6:
      if (GameFlag_IsSet(53)) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}
