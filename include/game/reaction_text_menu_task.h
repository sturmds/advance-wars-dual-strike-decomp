#ifndef GAME_REACTION_TEXT_MENU_TASK_H
#define GAME_REACTION_TEXT_MENU_TASK_H

#include "types.h"

#define REACTION_TEXT_SIZE 33 /* bytes of each reaction text in data_0229da8c */

/* Task of the menu that edits the six link reaction texts (data_0215fc0c is
 * its script). Reached by Task_FindByScript. */
typedef struct ReactionTextMenuTask {
  u8 pad_00[0x4e];
  s16 done; /* nonzero once the name entry for the chosen text has ended */
  u8 pad_50[0x52 - 0x50];
  u16 selected; /* index of the reaction text being edited, 0-5 */
} ReactionTextMenuTask;

#endif
