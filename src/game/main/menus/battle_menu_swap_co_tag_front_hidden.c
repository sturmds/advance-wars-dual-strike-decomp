#include "types.h"
#include "game/Unk_02080440.h"

#define CURRENT_PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])

/* Battle menu entry test: 1 hides the entry. Needs a back CO. Shown while the
 * front CO half of a tag power runs; its handler (Menu_SwapCo) swaps front
 * and back CO. */
s8 BattleMenu_SwapCoTagFrontHidden(void) {
  if (CURRENT_PLAYER.back.id == 0) {
    return 1;
  }
  return CURRENT_PLAYER.tagPowerHalf != TAG_POWER_FRONT;
}
