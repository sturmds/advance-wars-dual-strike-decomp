#include "types.h"
#include "game/play_records.h"

/* Lowers gPlayRecords.points to limit when it is higher. */
void PlayRecords_CapPoints(u32 limit) {
  if (gPlayRecords.points >= limit) {
    gPlayRecords.points = limit;
  }
}
