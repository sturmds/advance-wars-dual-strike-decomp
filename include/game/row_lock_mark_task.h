#ifndef GAME_ROW_LOCK_MARK_TASK_H
#define GAME_ROW_LOCK_MARK_TASK_H

#include "types.h"

/* Fields of the tasks of scripts gRowLockMarkRow1Script, gRowLockMarkRow2Script,
 * gRowLockMarkRow3Script and gRowLockMarkRow4Script: one per combat setup row (1 to 4).
 * RowLockMark_Start starts them when a row is locked and RowLockMark_Remove resumes
 * them when it is unlocked. Each draws one sprite on the screen of
 * data_027c0288.unk_04. The first 0x2c bytes belong to the task system
 * (Task_Create). */
typedef struct RowLockMarkTask {
    u8 pad_00[0x2c];
    int x;                             /* top left of the sprite at normal size */
    int y;
    u8 pad_34[0x4c - 0x34];
    s16 frame;
    u8 pad_4e[0x52 - 0x4e];
    u16 affineIndex;                   /* OBJ affine matrix 25 to 28 */
} RowLockMarkTask;

#endif
