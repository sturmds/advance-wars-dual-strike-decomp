#ifndef GAME_UNK_02088D04_H
#define GAME_UNK_02088D04_H

#include "types.h"
#include "game/BgControl.h"

/* One sprite queued for drawing. Each sprite list starts with an entry
 * of this type whose next field points to the newest sprite. */
typedef struct SpriteEntry {
    struct SpriteEntry *next;
    u16 x;                             /* OAM attribute 1 bits: x and flip flags */
    u16 y;                             /* OAM attribute 0 bits: y and above */
    u16 unk_08;
    u16 unk_0a;
    const void *unk_0c;
} SpriteEntry;

/* Pointed to by data_027c0280 (DTCM). */
typedef struct Unk_027c0280 {
    u8 pad_00[0x08];
    u8 *unk_08;
    u8 pad_0c[0x18 - 0x0c];
    void *textPrinter;                 /* text, position and target map of the text being printed */
    struct IconBank *iconBanks;        /* ICON_BANK_COUNT banks */
    u8 pad_20[0x24 - 0x20];
    SpriteEntry *spriteLists;
    SpriteEntry *spriteFree;           /* next unused entry */
    u8 pad_2c[0x6c - 0x2c];
    u16 dispCnt;                       /* DISPCNT bits 0-15 (bit 8-12: BG0-3 and OBJ on) */
    BgControl bgCnt[4];                /* BG0CNT-BG3CNT */
    u8 pad_76[0x90 - 0x76];
    u8 blendTopBg[4];                  /* BLDCNT first target, per BG */
    u16 blendTopObj : 1;
    u16 blendTopBackdrop : 1;
    u16 blendMode : 2;                 /* 0 none, 1 alpha */
    u16 : 12;
    u8 blendBottomBg[4];               /* BLDCNT second target, per BG */
    u16 blendBottomObj : 1;
    u16 : 15;
    u8 pad_9c[0xbc - 0x9c];
    u16 blendEva;
    u16 blendEvb;
    u16 blendEvy;
    u16 bgScrollX[4];
    u16 bgScrollY[4];
    u8 pad_d2[0xdd - 0xd2];
    u8 unk_dd;
    u8 pad_de[0xe1 - 0xde];
    /* Four layers of 0x14 bytes from 0xe0: +1 the BG they use, +0xc
     * their tile map buffer. */
    u8 layer0Bg;
    u8 pad_e2[0xe4 - 0xe2];
    u8 *unk_e4;                        /* tile data that the digit font loads into */
    u8 pad_e8[0xec - 0xe8];
    u16 *layer0Map;
    u8 pad_f0[0xf5 - 0xf0];
    u8 layer1Bg;
    u8 pad_f6[0xf8 - 0xf6];
    void *unk_f8;
    u8 pad_fc[0x100 - 0xfc];
    u16 *layer1Map;
    u8 pad_104[0x109 - 0x104];
    u8 layer2Bg;
    u8 pad_10a[0x114 - 0x10a];
    u16 *layer2Map;
    u8 pad_118[0x11d - 0x118];
    u8 layer3Bg;
    u8 pad_11e[0x128 - 0x11e];
    u16 *layer3Map;
    u8 pad_12c[0x130 - 0x12c];
    u8 *bgVram;                        /* BG character base 0 */
    u8 *objVram;                       /* CO pictures load here, 32 bytes per tile */
} Unk_027c0280;

/* data_027c0288 (DTCM). */
typedef struct Unk_027c0288 {
    void *unk_00;
    void *unk_04;                      /* loaded into data_027c027c and data_027c0280 */
} Unk_027c0288;

/* A 0x6c-byte object found by Task_FindByScript. */
typedef struct Unk_02023724 {
    u8 pad_00[0x32];
    u8 unk_32;                         /* selected entry of a list */
} Unk_02023724;

#endif
