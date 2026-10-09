#ifndef GAME_UNK_02290718_H
#define GAME_UNK_02290718_H

#include "types.h"
#include "game/co_data.h"
#include "game/co_save.h"

/* data_02290718 (main bss), 0x38 bytes before the Force Rank table
 * that func_02017abc reads. */
typedef struct Unk_02290718 {
    u8 pad_00[0x0f];
    u8 unk_0f;
    u8 pad_10[0x12 - 0x10];
    u8 unk_12;                         /* copied to MenuState.setup.unk_2e1 */
    u8 pad_13[0x15 - 0x13];
    u8 unk_15;                         /* copied to MenuState.setup.unk_2e2 */
    u8 pad_16[0x38 - 0x16];
    CoSaveRecord coRecords[CO_COUNT];  /* by CO id */
    u8 pad_after_records[0x3c0 - 0x38 - CO_COUNT * sizeof(CoSaveRecord)];
    u32 humanFundsSpent;               /* funds spent by human players, capped at 999999999 (Player_SpendFunds) */
} Unk_02290718;

extern Unk_02290718 data_02290718;

#endif
