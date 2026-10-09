#ifndef GAME_UNK_02088D04_H
#define GAME_UNK_02088D04_H

#include "types.h"
#include "game/BgControl.h"

/* Pointed to by data_027c0280 (DTCM). */
typedef struct Unk_027c0280 {
    u8 pad_00[0x08];
    u8 *unk_08;
    u8 pad_0c[0x6e - 0x0c];
    BgControl bgControl[8];            /* indexed by BG number; values for the BGxCNT registers */
    u8 pad_7e[0x90 - 0x7e];
    u8 unk_90[4];                      /* indexed by BG number */
    u16 unk_94_0 : 1;
    u16 unk_94_1 : 1;
    u16 unk_94_2 : 2;
    u16 unk_94_4 : 12;
    u8 unk_96[4];                      /* indexed by BG number */
    u16 unk_9a_0 : 1;
    u16 unk_9a_1 : 15;
    u8 pad_9c[0xbc - 0x9c];
    u16 unk_bc;
    u16 unk_be;
    u16 unk_c0;
    u16 unk_c2[4];                     /* indexed by unk_e1, unk_f5 and unk_109; a scroll offset per background (a guess) */
    u8 pad_ca[0xe1 - 0xca];
    u8 unk_e1;                         /* a BG number */
    u8 pad_e2[0xec - 0xe2];
    void *unk_ec;                      /* passed to the text drawing calls */
    u8 pad_f0[0xf5 - 0xf0];
    u8 unk_f5;                         /* a BG number */
    u8 pad_f6[0xf8 - 0xf6];
    void *unk_f8;
    u8 pad_fc[0x109 - 0xfc];
    u8 unk_109;                        /* a BG number */
    u8 pad_10a[0x114 - 0x10a];
    void *unk_114;
    u8 pad_118[0x11d - 0x118];
    u8 unk_11d;                        /* a BG number */
    u8 pad_11e[0x130 - 0x11e];
    u8 *unk_130;                       /* character base 0 of a BG; base n starts n * 0x4000 bytes on */
    u8 *charBase;                      /* CO pictures load here, 32 bytes per tile */
} Unk_027c0280;

/* data_027c0288 (DTCM). */
typedef struct Unk_027c0288 {
    void *unk_00;
    void *unk_04;                      /* loaded into data_027c027c and data_027c0280 */
} Unk_027c0288;

/* A 0x6c-byte object found by func_02023724. */
typedef struct Unk_02023724 {
    u8 pad_00[0x32];
    u8 unk_32;                         /* selected entry of a list */
} Unk_02023724;

#endif
