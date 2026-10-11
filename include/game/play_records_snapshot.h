#ifndef GAME_PLAY_RECORDS_SNAPSHOT_H
#define GAME_PLAY_RECORDS_SNAPSHOT_H

#include "types.h"

/* data_022a7fc8 (main bss): copy of the first two words of gPlayRecords,
 * taken by func_02103c84. */
typedef struct PlayRecordsSnapshot {
    int unk_00;
    int unk_04;
} PlayRecordsSnapshot;

extern PlayRecordsSnapshot data_022a7fc8;

#endif
