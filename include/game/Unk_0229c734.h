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

#define SCORE_ROW_COUNT 22
#define RESULT_POINT_PART_COUNT 5

/* Pointed to by data_0229c734 (main bss). */
typedef struct Unk_0229c734 {
    ScoreRow rows[SCORE_ROW_COUNT];
    u16 partTile[RESULT_POINT_PART_COUNT]; /* first OBJ tile of each graphics part, 0xffff until loaded */
    u8 pad_8e[0x90 - 0x8e];
    u8 *gfxFile;                       /* /ohashi/res_result_point_obj */
    u16 nextTile;                      /* next free OBJ tile */
    u16 paletteSlot;                   /* first OBJ palette used */
    u16 unk_98;
    s16 unk_9a;                        /* set by func_0203813c, read by func_02038150 */
    u8 unk_9c;
    u8 unk_9d;                         /* set by func_0203b194 */
    u8 unk_9e;
    u8 unk_9f;
    u8 rowCount;
    u8 nations[2];                     /* Co_GetNation of the two COs in data_02160abc */
    u8 pad_a3[0xa4 - 0xa3];
    int unk_a4;                        /* set to 1 by ScriptTask_ResumeWithValue, read by func_02038164 */
} Unk_0229c734;

/* The task that runs the result point script (data_0215a44c). */
typedef struct ResultPointTask {
    u8 pad_00[0x32];
    s16 frame;                         /* frames the current slide has run */
    u8 pad_34[0x38 - 0x34];
    s16 unk_38;
    s16 value;                         /* set by ResultPoint_RestartScript; the slide's frame count */
} ResultPointTask;

extern Unk_0229c734 *data_0229c734;

#endif
