#include "types.h"
#include "game/play_records.h"
#include "game/co_data.h"

/* Saves experience for CO `co`, at most 100000. Clones (CO_CLONE) have none. */
void Co_SetExperience(u32 co, u32 experience) {
  if (co & CO_CLONE) {
    return;
  }
  if (experience >= 100000) {
    experience = 100000;
  }
  gPlayRecords.coRecords[co].experience = experience;
}
