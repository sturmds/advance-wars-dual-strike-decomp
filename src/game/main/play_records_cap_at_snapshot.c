#include "types.h"
#include "game/play_records.h"
#include "game/play_records_snapshot.h"

/* Lowers the first two counters of gPlayRecords to the values in
 * gPlayRecordsSnapshot when they are higher. */
void PlayRecords_CapAtSnapshot(void) {
  PlayRecords_CapPoints(gPlayRecordsSnapshot.points);
  func_02017c88(gPlayRecordsSnapshot.unk_04);
}
