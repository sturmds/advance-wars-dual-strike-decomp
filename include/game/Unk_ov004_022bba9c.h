#ifndef GAME_UNK_OV004_022BBA9C_H
#define GAME_UNK_OV004_022BBA9C_H

#include "types.h"

/* Work area of the overlay 4 task that runs func_ov004_022bba9c (a second
 * scene; the first one uses Unk_ov004_022bf158). The task keeps a pointer to
 * it at offset 0x2c. */
typedef struct Unk_ov004_022bba9c {
    u32 unk_000;
    u8 pad_004[0xad8 - 0x4];
    struct Task *childTasks[16];       /* destroyed together by Ov004_DestroyChildTasks */
    u8 pad_b18[0xbf4 - 0xb18];
    struct Task *unk_bf4;              /* the six tasks from 0xbf4 to 0xc08 are destroyed by func_ov004_022bdb7c */
    struct Task *unk_bf8;
    struct Task *unk_bfc;
    struct Task *unk_c00;
    struct Task *unk_c04;
    struct Task *unk_c08;
    int unk_c0c;                       /* func_ov004_022bc984 draws only while this is nonzero */
} Unk_ov004_022bba9c;

#endif
