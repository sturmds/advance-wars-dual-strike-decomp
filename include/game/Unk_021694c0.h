#ifndef GAME_UNK_021694C0_H
#define GAME_UNK_021694C0_H

#include "types.h"

/* Pointed to by data_021694c0; used by many functions near 0x020f1b0c-0x020f3604. */
typedef struct Unk_021694c0 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0c;
    int unk_10;
    int unk_14;
    u8 pad_18[4];
    int unk_1c;
    int unk_20;
    int unk_24;
    int unk_28;
    int unk_2c;
    int unk_30;
    u8 pad_34[4];
    int unk_38;
    int unk_3c;
} Unk_021694c0;

/* Task of the effect scripts in the data_021694c0 scene. offsetX and offsetY
 * are added to unk_24 and unk_28 of the data_021694c0 record to place the
 * sprite. */
typedef struct Unk_021694c0Task {
    u8 pad_00[0x2c];
    int offsetX;
    int offsetY;
    u8 pad_34[0x50 - 0x34];
    void *sprite;                      /* handle from func_02022478 */
    u8 pad_54[0x64 - 0x54];
    s16 frame;
} Unk_021694c0Task;

#endif
