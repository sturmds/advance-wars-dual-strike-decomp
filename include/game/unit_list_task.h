#ifndef GAME_UNIT_LIST_TASK_H
#define GAME_UNIT_LIST_TASK_H

#include "types.h"

/* The task of the unit list menu (script data_0215fad0), from offset 0x29
 * of its Task. The list rows are in data_0229c878 (UnitList_Build). */
typedef struct UnitListTask {
    u8 pad_00[0x29];
    u8 sortMode;                       /* UnitList_Build's sort mode, 0-3 */
    u8 reverse;                        /* 1: the sort runs the other way */
    u8 scroll;                         /* in half rows; steps toward topRow * 2 */
    u8 topRow;                         /* first list row on screen */
    u8 scrollDelay;                    /* frames until scroll takes its next step */
    u8 unitCount;                      /* from UnitList_ClearAndCount */
    u8 unk_2f;                         /* set to 10 when the menu opens */
    u8 unk_30;                         /* set to 10 when the menu opens */
    u8 pad_31[0x34 - 0x31];
    int unk_34;
    int unk_38;
    u16 unk_3c;
} UnitListTask;

#define UNIT_LIST_VISIBLE_ROWS 6       /* list rows on screen at once, one button each */

#endif
