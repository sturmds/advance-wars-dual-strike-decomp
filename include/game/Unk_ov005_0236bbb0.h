#ifndef GAME_UNK_OV005_0236BBB0_H
#define GAME_UNK_OV005_0236BBB0_H

#include "types.h"

/* Overlay 5 work block (0x1d68 bytes) that func_ov005_02366eec allocates
 * into Ov005Task.work and data_ov005_0236bbb0: Hachi's shop, a list of
 * items with prices (from the 28-byte records at data_0216cb60), six rows
 * per page. */
typedef struct Unk_ov005_0236bbb0 {
    u8 pad_00[0x4];
    int unk_04;
    u32 unk_08;
    int unk_0c;
    int unk_10;
    int unk_14;
    int redrawRows;                    /* 1: func_ov005_0236775c clears the rows first */
    int unk_1c;
    u32 shopItemCount;                 /* records of data_0216cb60 that func_ov005_02367ad8 scans */
    u8 pad_24[0x10c - 0x24];
    int unk_10c;
    int unk_110;
    u8 pad_114[0x1d64 - 0x114];
    struct Task *songTask;             /* func_020fcbd8(12, 0): fades to song 12 */
} Unk_ov005_0236bbb0;

#endif
