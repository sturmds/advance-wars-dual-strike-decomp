#ifndef GAME_TAG_POWER_SCROLL_TASK_H
#define GAME_TAG_POWER_SCROLL_TASK_H

#include "types.h"

/* Fields of the Task that scrolls the Tag Power cut-in background on both
 * screens (func_02055e40 clears them, TagPowerScreen_ScrollBackground
 * advances them). */
typedef struct TagPowerScrollTask {
  u8 pad_00[0x58];
  int layer1Y; /* BG layer 1 scroll, +24 per frame */
  int layer2Y; /* BG layer 2 scroll, +12 per frame */
} TagPowerScrollTask;

#endif
