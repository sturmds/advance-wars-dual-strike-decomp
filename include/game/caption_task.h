#ifndef GAME_CAPTION_TASK_H
#define GAME_CAPTION_TASK_H

#include "types.h"

#define CAPTION_TEXT 0x7d000020         /* the caption the tasks of data_0215b06c show */

/* Fields of the tasks of script data_0215b06c, which draw a centered line
 * of text with sprites around it on the screen of data_027c0288.unk_04.
 * The first 0x4c bytes belong to the task system (Task_Create). */
typedef struct CaptionTask {
    u8 pad_00[0x4c];
    s16 frame;
} CaptionTask;

#endif
