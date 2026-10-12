#include "types.h"
#include "game/play_records.h"

/* Saved experience of CO `co`. Ids with bit 7 set have none. */
u32 Co_GetExperience(u32 co) {
  if (co & 0x80) {
    return 0;
  }
  return gPlayRecords.coRecords[co].experience;
}
