#include "types.h"
#include "game/Unk_02290718.h"

/* Saved experience of CO `co`. Ids with bit 7 set have none. */
u32 Co_GetExperience(u32 co)
{
    if (co & 0x80) {
        return 0;
    }
    return data_02290718.coRecords[co].experience;
}
