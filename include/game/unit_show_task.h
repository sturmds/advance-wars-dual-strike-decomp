#ifndef GAME_UNIT_SHOW_TASK_H
#define GAME_UNIT_SHOW_TASK_H

#include "types.h"
#include "game/map_unit.h"

/* Task made by UnitShowTask_Create for a unit the AI is showing. The first
 * 0x30 bytes belong to the task system (Task_Create). */
typedef struct UnitShowTask {
    u8 pad_00[0x2c];
    void *sprite;                      /* 2D or 3D sprite handle */
    MapUnit *unit;                     /* flags have SELECTED and LOCKED set while the task lives */
    u8 scriptPos;                      /* next byte of script */
    u8 unk_35;
    u8 pad_36[0x38 - 0x36];
    s8 frame;                          /* sprite frame last set with func_02021f44 */
    u8 pad_39[0x3c - 0x39];
    u16 waitFrames;                    /* UnitShowTask_RunScript runs only while this is 0 */
    u8 pad_3e[0x40 - 0x3e];
    s16 x;                             /* pixels: tile * 16 */
    s16 y;
    u8 pad_44[0x4c - 0x44];
    s8 script[0x20];                   /* commands of UnitShowTask_RunScript */
} UnitShowTask;

#endif
