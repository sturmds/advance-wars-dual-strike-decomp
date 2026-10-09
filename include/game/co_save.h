#ifndef GAME_CO_SAVE_H
#define GAME_CO_SAVE_H

#include "types.h"

/* Saved progress of one CO, indexed by CO id (data_02290750, main bss). */
typedef struct CoSaveRecord {
    u32 experience : 19;               /* func_020179c0 caps it at 100000 */
    u32 unk_0_19 : 13;
    u8 pad_04[0x1c - 0x04];
} CoSaveRecord;

extern CoSaveRecord data_02290750[];

#endif
