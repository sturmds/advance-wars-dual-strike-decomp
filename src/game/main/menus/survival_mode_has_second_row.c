#include "types.h"
#include "game/Unk_022a7b80.h"
#include "game/unlocks.h"

extern Unk_022a7b80 data_022a7b80;

/* TRUE when the mode pair of the current mode (0/3, 2/5, 1/4) offers its
 * second variant: game flag 47, 48 or 49 is set. */
BOOL SurvivalMode_HasSecondRow(void) {
  switch (data_022a7b80.mode) {
    case 0:
    case 3:
      if (GameFlag_IsSet(47)) {
        return TRUE;
      }
      break;
    case 2:
    case 5:
      if (GameFlag_IsSet(48)) {
        return TRUE;
      }
      break;
    default:
      if (GameFlag_IsSet(49)) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}
