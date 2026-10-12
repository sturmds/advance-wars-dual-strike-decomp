#include "types.h"
#include "game/play_records.h"
#include "game/play_records_snapshot.h"

/* Copies points and unk_04 of gPlayRecords so PlayRecords_CapAtSnapshot can
 * put them back down later. */
PlayRecordsSnapshot* PlayRecords_TakeSnapshot(void) {
  gPlayRecordsSnapshot.points = gPlayRecords.points;
  gPlayRecordsSnapshot.unk_04 = gPlayRecords.unk_04;
  return &gPlayRecordsSnapshot;
}
