#ifndef GAME_STAGGERED_TASK_H
#define GAME_STAGGERED_TASK_H

#include "types.h"

/* Fields of a Task whose update handles one pending item every 16 frames
 * (see func_020317f4). */
typedef struct StaggeredTask {
  u8 pad_00[0x4c];
  s16 frame;     /* 0 to 15: an item is handled when it is 0 */
  s16 itemsLeft; /* script resumes when this reaches 0 */
  u8 pad_50[0x52 - 0x50];
  u16 pending; /* bit n set: item n not handled yet */
} StaggeredTask;

#endif
