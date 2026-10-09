#include "types.h"
#include "game/co_data.h"
#include "game/player.h"

/* CoTagPartner.stars of CO 0's first partner; the code addresses the field
 * through this label. */
extern u16 data_0215367a;

/* Tag stars (1-3) of a CO paired with a partner, or 0 for a pair with no
 * entry. */
int Co_GetTagStars(int co, int partner)
{
    int offset = (co & PLAYER_CO_ID_MASK) * sizeof(CoRecord);
    int i;
    CoRecord *record = (CoRecord *)((u8 *)gCoTable + offset);

    for (i = 0; i < 3; i++) {
        if (partner == record->tagPartners[i].co) {
            return *(u16 *)((u8 *)&data_0215367a + offset + i * sizeof(CoTagPartner));
        }
    }
    return 0;
}
