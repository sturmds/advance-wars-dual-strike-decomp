#include "types.h"
#include "game/co_data.h"
#include "game/player.h"

/* Quotes and power name for a CO paired with a partner, or NULL for a pair
 * with no entry. */
const CoTagRecord* Co_GetTagRecord(int co, int partner) {
  const CoTagRecord* found;
  int i;
  CoRecord* record;
  /* needed to match: declarations first, then assignments */
  found = NULL;
  record = &gCoTable[co & PLAYER_CO_ID_MASK];

  for (i = 0; i < 3; i++) {
    if ((partner & PLAYER_CO_ID_MASK) == record->tagPartners[i].co) {
      found = record->tagPartners[i].record;
    }
  }
  return found;
}
