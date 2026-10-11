#ifndef GAME_UNK_0229C734_H
#define GAME_UNK_0229C734_H

#include "types.h"

/* One row of the score list that ScoreList_AddRow fills. */
typedef struct ScoreRow {
    u16 points;
    u8 kind;                           /* ScoreList_AddRow's argument, 1-21 */
    u8 count;                          /* the number the points come from, or 0 */
    u8 height;                         /* 16 or 32; func_0203ae00 fits rows into 144 */
    u8 pad_05;
} ScoreRow;

/* Pointed to by data_0229c734 (main bss). */
typedef struct Unk_0229c734 {
    ScoreRow rows[24];
    u8 pad_90[0x9a - 0x90];
    s16 unk_9a;                        /* set by func_0203813c, read by func_02038150 */
    u8 pad_9c[0x9d - 0x9c];
    u8 unk_9d;                         /* set by func_0203b194 */
    u8 pad_9e[0xa0 - 0x9e];
    u8 rowCount;
    int unk_a4;                        /* set to 1 by ScriptTask_ResumeWithValue, read by func_02038164 */
} Unk_0229c734;

extern Unk_0229c734 *data_0229c734;

#endif
