#ifndef GAME_PLAY_RECORDS_SNAPSHOT_H
#define GAME_PLAY_RECORDS_SNAPSHOT_H

#include "types.h"

/* gPlayRecordsSnapshot (main bss): copy of the first two words of gPlayRecords,
 * taken by PlayRecords_TakeSnapshot. */
typedef struct PlayRecordsSnapshot {
  u32 points; /* PlayRecords.points */
  u32 unk_04; /* PlayRecords.unk_04 */
} PlayRecordsSnapshot;

extern PlayRecordsSnapshot gPlayRecordsSnapshot;

#endif
