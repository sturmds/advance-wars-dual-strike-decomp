#include "types.h"
#include "game/Unk_02160ac0.h"

/* Score multiplier of the Combat mode level (data_02160ac0->unk_84):
 * 1, 2 or 3 for levels 0 to 2. */
int ScoreList_GetMultiplier(void) {
  switch (data_02160ac0->unk_84) {
    case 0:
    default:
      return 1;
    case 1:
      return 2;
    case 2:
      return 3;
  }
}
