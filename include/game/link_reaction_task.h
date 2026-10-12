#ifndef GAME_LINK_REACTION_TASK_H
#define GAME_LINK_REACTION_TASK_H

#include "types.h"

/* Fields of the tasks of script data_0216050c: a line of text sprites that
 * scrolls across the screen to show a reaction another player sent over the
 * link. The first 0x2a bytes belong to the task system (Task_Create). */
typedef struct LinkReactionTask {
  u8 pad_00[0x2a];
  s16 x; /* screen x of the first glyph; starts at 256 and falls by 2 a frame */
  s16 y; /* screen y; one 24 pixel row per link slot */
  u8 slot;   /* link slot of the sender, from 1 */
  u8 button; /* which reaction button it answers, 0 to 5 */
  u8 glyphCount;
  u8 glyphs[0x6c -
            0x31]; /* glyph numbers; each is drawn from tile 0x94 + 2 * glyph */
} LinkReactionTask;

#endif
