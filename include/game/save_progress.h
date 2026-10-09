#ifndef GAME_SAVE_PROGRESS_H
#define GAME_SAVE_PROGRESS_H

#include "types.h"
#include "game/co_data.h"
#include "game/co_save.h"

/* data_02291774 (main bss): the buffer that Save_LoadRecord(0, ...) fills.
 * Save_LoadProgress copies its CO records and Force Rank sets into the live
 * progress data. */
typedef struct SaveProgress {
    u8 pad_00[0x58];
    u32 unk_58;                        /* Save_LoadProgress passes it to func_02017ca0 */
    u32 unk_5c;                        /* passed to func_02017c88 */
    u8 pad_60[0x90 - 0x60];
    CoSaveRecord coRecords[CO_COUNT];  /* by CO id */
    u8 pad_after_records[0x10c4 - 0x90 - CO_COUNT * sizeof(CoSaveRecord)];
    ForceRankSet profileSets[1][96];   /* [Unk_02290118.unk_00][CO id]; more profiles may follow */
} SaveProgress;

extern SaveProgress data_02291774;

#endif
