#ifndef GAME_UNK_022A7A9C_H
#define GAME_UNK_022A7A9C_H

#include "types.h"

/* data_022a7a9c (main bss). ScoreList_AddRow reads these bytes as points
 * (0x00-0x02) or as counts it multiplies into points (0x08-0x0d). */
typedef struct Unk_022a7a9c {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 pad_03[0x04 - 0x03];
    u8 unk_04;                         /* func_02033088 copies it to its task's unk_64 */
    u8 pad_05;
    u16 score;                         /* saved by BattleRecord_SaveWin as a Campaign or War Room record */
    u8 unk_08;
    u8 unk_09;
    u8 unk_0a;
    u8 unk_0b;
    u8 unk_0c;
    u8 unk_0d;
    u8 pad_0e[0x10 - 0x0e];
    s32 unk_10;                        /* entry 0: points the win/lose screen shows */
    s32 unk_14;                        /* BattleEnd_AwardCoins adds unk_14 - unk_18 to the coin total */
    s32 unk_18;
    u16 frontCo;                       /* CO id in bits 0-6 (WinLoseScreen_Init) */
    u16 backCo;                        /* CO id in bits 0-6; 0: no tag partner */
    u8 pad_20[0x28 - 0x20];            /* WinLoseScreen_Init indexes 0x28-byte entries by player slot */
} Unk_022a7a9c;

extern Unk_022a7a9c data_022a7a9c;

/* 0x28-byte records from data_022a7ab8. The low 7 bits of each of the
 * first two halfwords hold a CO id; the second halfword has its own
 * symbol, data_022a7aba, so it is read through that. */
typedef struct Unk_022a7ab8 {
    u16 unk_00;
    u8 pad_02[0x28 - 0x02];
} Unk_022a7ab8;

extern Unk_022a7ab8 data_022a7ab8[];
extern Unk_022a7ab8 data_022a7aba[];  /* the same records, from their halfword at +2 */

#endif
