#include "types.h"
#include "game/co_data.h"
#include "game/player.h"

/* Firepower in percent (100 = no change) that co gets when tagged with
 * partner. Callers subtract 100 to get the bonus. */
u8 Co_GetTagFirepower(int co, int partner) {
  return gCoTable[co & PLAYER_CO_ID_MASK]
      .tagFirepower[partner & PLAYER_CO_ID_MASK];
}
