#ifndef GAME_WIN_LOSE_H
#define GAME_WIN_LOSE_H

#include "types.h"

/* data_022a7a88 (main bss): the players on each side of the win/lose
 * screen. Side B is empty when the screen shows one side only. */
typedef struct WinLoseSides {
    s8 sideA[4];                       /* player slots; index into data_022a7a9c */
    s8 sideB[4];
    u8 countA;                         /* players in sideA */
    u8 countB;                         /* players in sideB; 0: one side only */
    u8 pad_0a[0x0c - 0x0a];
    s32 unk_0c;                        /* points, added to data_022a7a9c's entry 0 unk_10 */
} WinLoseSides;

extern WinLoseSides data_022a7a88;

/* The points total (WinLoseTask.unk_5c) stops here. */
#define WIN_LOSE_POINTS_MAX 99999

/* The task that runs WinLoseScreen_Init, 0x6c bytes like Task. */
typedef struct WinLoseTask {
    u8 pad_00[0x4c];
    s16 unk_4c;
    u16 scoreWidth;                    /* from func_0203f300: 24 plus 4 per digit after the first */
    u8 pad_50[0x58 - 0x50];
    s32 unk_58;                        /* points from data_022a7a88.unk_0c */
    s32 unk_5c;                        /* points, at most 99999 */
    u8 pad_60[0x64 - 0x60];
    s16 portraitsA;                    /* two sides: CO pictures of side A; one side: bit per player with a tag partner */
    s16 portraitsB;                    /* CO pictures of side B */
    s16 portraitsMax;                  /* the larger of the two */
} WinLoseTask;

#endif
