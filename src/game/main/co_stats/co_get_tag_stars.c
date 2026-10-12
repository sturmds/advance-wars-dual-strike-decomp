#include "types.h"
#include "game/co_data.h"
#include "game/player.h"

/* Tag stars (1-3) of a CO paired with a partner, or 0 for a pair with no
 * entry. */
int Co_GetTagStars(int co, int partner) {
  int offset = (co & PLAYER_CO_ID_MASK) * sizeof(CoRecord);
  int i;
  CoRecord* record = (CoRecord*)((u8*)gCoTable + offset);

  for (i = 0; i < 3; i++) {
    if (partner == record->tagPartners[i].co) {
      /* needed to match: the address is rebuilt from field 0 of CO 0, not from record */
      return *(u16*)((u8*)&gCoTable[0].tagPartners[0].stars + offset +
                     i * sizeof(CoTagPartner));
    }
  }
  return 0;
}
