#ifndef GAME_UNK_02088D04_H
#define GAME_UNK_02088D04_H

#include "types.h"

/* Pointed to by data_027c0280 (DTCM). */
typedef struct Unk_027c0280 {
    u8 pad_00[0xec];
    void *unk_ec;                      /* passed to the text drawing calls */
    u8 pad_f0[0x134 - 0xf0];
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
