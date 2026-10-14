#include "types.h"
#include "game/unlocks.h"
#include "game/save_progress.h"

/* Game progress flags, split over three bit arrays by id range. */
extern u8 gGameFlagsLow[]; /* ids 0-31 */
/* ids 32-94 are in gGameFlagsMid */
extern u8 gGameFlagsHigh[]; /* ids 95-158 */

BOOL GameFlag_IsSet(int id) {
  if (id >= 95 && id < 159) {
    id -= 95;
    /* needed to match: operands in the other order from the two blocks below */
    return ((1 << (id & 7)) & gGameFlagsHigh[id >> 3]) != 0;
  }
  if (id >= 32 && id < 95) {
    id -= 32;
    return (gGameFlagsMid.value[id >> 3] & (1 << (id & 7))) != 0;
  }
  if (id >= 0 && id < 32) {
    return (gGameFlagsLow[id >> 3] & (1 << (id & 7))) != 0;
  }
  return FALSE;
}
