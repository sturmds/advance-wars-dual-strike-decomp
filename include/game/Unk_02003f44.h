#ifndef GAME_UNK_02003F44_H
#define GAME_UNK_02003F44_H

#include "types.h"

/*
 * Steps through a list of s16 pairs (unk_4c, ended by 0x7fff) and copies
 * each pair to data_0216e2ac / data_0216e2a4; see func_02003e84.
 */
typedef struct Unk_02003f44 {
    u8 pad_00[0x44];
    s16 unk_44; /* step limit, -1 for none (func_02003e84) */
    u8 pad_46[0x4c - 0x46];
    s16 *unk_4c; /* the pair list */
    u8 pad_50[0x54 - 0x50];
    u32 unk_54; /* index of the next pair */
    u32 unk_58; /* steps taken */
} Unk_02003f44;

#endif
