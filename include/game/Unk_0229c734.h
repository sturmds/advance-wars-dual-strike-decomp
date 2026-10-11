#ifndef GAME_UNK_0229C734_H
#define GAME_UNK_0229C734_H

#include "types.h"

/* One row of the score list that ScoreList_AddRow fills. */
typedef struct ScoreRow {
    u16 points;
    u8 kind;                           /* ScoreList_AddRow's argument, 1-21 */
    u8 count;                          /* the number the points come from, or 0 */
    u8 height;                         /* 16 or 32; ScoreList_FitFirstPage fits rows into 144 */
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
    u16 pageMaxPoints;                 /* most points of a row on the shown page (ScoreList_UpdatePageMax) */
    s16 tally;                         /* points counted up on the result screen, later handed out to the COs (CoExp_TallyStep) */
    u8 firstRow;                       /* first score row on the shown page */
    u8 pageRowCount;                   /* rows on the shown page; also set by func_0203b194 */
    u8 topMargin;                      /* pixels above the first row, centring the page */
    u8 pageIndex;                      /* page the tally counts; 0: the first */
    u8 rowCount;
    u8 nations[2];                     /* Co_GetNation of the two COs in data_02160abc */
    u8 pad_a3[0xa4 - 0xa3];
    int unk_a4;                        /* set to 1 by ScriptTask_ResumeWithValue, read by func_02038164 */
} Unk_0229c734;

/* The task that runs the result point script (data_0215a44c). */
typedef struct ResultPointTask {
    u8 pad_00[0x32];
    s16 frame;                         /* frames the current slide has run */
    s16 tallyPopFrame;                 /* frame of the tally's pop-in (ResultPoint_DrawTallyPopIn) */
    s16 totalPoints;                   /* sum of the points of the rows in use */
    s16 countStep;                     /* points value the tally has counted up to on this page */
    s16 value;                         /* set by ResultPoint_RestartScript; the slide's frame count */
    u8 rank;                           /* ScoreList_GetPanelSize of totalPoints */
} ResultPointTask;

extern Unk_0229c734 *data_0229c734;

s16 ResultPoint_GetTally(void);
void ResultPoint_SetTally(int points);

#endif
