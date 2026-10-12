#ifndef GAME_CAPTION_TASK_H
#define GAME_CAPTION_TASK_H

#include "types.h"

#define CAPTION_TEXT \
  0x7d000020 /* the caption the tasks of data_0215b06c show */

/* The two lines the tasks of script data_02159da8 show, also through a
 * CaptionTask: line A at y 80 in the sprite font, line B at y 72 through
 * CaptionLetter_GetAdvance. */
#define TWO_LINE_CAPTION_TEXT_A 0x7d00001e
#define TWO_LINE_CAPTION_TEXT_B 0x7d00001f

/* Fields of the tasks of script data_0215b06c, which draw a centered line
 * of text with sprites around it on the screen of data_027c0288.unk_04.
 * The first 0x4c bytes belong to the task system (Task_Create). */
typedef struct CaptionTask {
  u8 pad_00[0x4c];
  s16 frame;
} CaptionTask;

#endif
