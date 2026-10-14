#include "types.h"
#include "game/play_records.h"
#include "game/co_data.h"

/* Saved experience of CO `co`. Clones (CO_CLONE) have none. */
u32 Co_GetExperience(u32 co) {
  if (co & CO_CLONE) {
    return 0;
  }
  return gPlayRecords.coRecords[co].experience;
}
