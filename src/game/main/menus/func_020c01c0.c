#include "types.h"
#include "game/Unk_02080440.h"

BOOL Map_IsCoSelectPhase(void);
BOOL Campaign_IsHardMode(void);

/* Battle menu entry test (table at 0x02165d30, handler func_020bfd50): 1 hides
 * the entry. Hidden in the CO select phase. Outside the campaign and in hard
 * mode it is shown. In the normal campaign the mapId - 224 < 9 test decides;
 * it is true for every mapId below 233, so the entry shows from mapId 233 on. */
s8 func_020c01c0(void) {
  if (Map_IsCoSelectPhase()) {
    return 1;
  }
  if (MAP_STATE->gameMode != GAME_MODE_CAMPAIGN) {
    return 0;
  }
  if (Campaign_IsHardMode()) {
    return 0;
  }
  return MAP_STATE->unk_1c4->mapId - 224 < 9;
}
