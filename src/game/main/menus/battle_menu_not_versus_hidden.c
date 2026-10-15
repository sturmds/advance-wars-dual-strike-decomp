#include "types.h"
#include "game/Unk_02080440.h"

/* Battle menu entry test (table at 0x02165f10): 1 hides the entry in every
 * game mode but Versus. */
s8 BattleMenu_NotVersusHidden(void) {
  return MAP_STATE->gameMode != GAME_MODE_VERSUS;
}
