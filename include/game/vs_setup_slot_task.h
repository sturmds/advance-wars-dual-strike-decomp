#ifndef GAME_VS_SETUP_SLOT_TASK_H
#define GAME_VS_SETUP_SLOT_TASK_H

#include "types.h"

/* Task of one player slot on the Vs setup screen, made by VsSetup_StartPlayerTask.
 * The first 0x2c bytes belong to the task system (Task_Create). */
typedef struct VsSetupSlotTask {
    u8 pad_00[0x2c];
    s32 x;                             /* pixel position the slot's sprite is drawn at */
    s32 y;
    u8 pad_34[0x4c - 0x34];
    s16 frame;                         /* frames since the step started */
    u8 pad_4e[0x52 - 0x4e];
    u16 matrix;                        /* 25 + slot: OAM affine matrix of the slot sprite */
} VsSetupSlotTask;

#endif
