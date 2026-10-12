#include "types.h"
#include "game/task.h"

/* Tile position {x, y} of each choice marker: (0, 0), (5, 0), (10, 0), (16, 0).
 * data_021584ca is the same table from its first y. */
extern s16 data_021584c8[][2];
extern s16 data_021584ca[][2];

#define CURSOR(task) (*(s16*)&(task)->waitFrames)

/* The original was built without common subexpression elimination here: it
 * reads the marker table again for every store. */
#pragma opt_common_subs off

/* Redraws the cursor of the event choice prompt: clears the two-tile column
 * of every choice marker in the task's tile map, then puts tiles 0x1b1 and
 * 0x1b2 (palette from unk_40) at the marker of the current choice and calls
 * unk_3c. */
void EventChoice_DrawCursor(Task* task) {
  int i;
  u16(*tiles)[32] = *(u16(**)[32]) & task->unk_34;

  for (i = 0; i < 4; i++) {
    /* The clear scales the row by 32 rows where the draw below does not;
         * every marker sits in row 0, so both reach the same tiles. */
    tiles[data_021584c8[i][1] * 32][data_021584c8[i][0]] = 0;
    (&tiles[data_021584c8[i][1] * 32][data_021584c8[i][0]])[32] = 0;
  }
  tiles[data_021584ca[CURSOR(task)][0]][data_021584c8[CURSOR(task)][0]] =
      task->unk_40 | 0x1b1;
  (&tiles[data_021584ca[CURSOR(task)][0]][data_021584c8[CURSOR(task)][0]])[32] =
      task->unk_40 | 0x1b2;
  task->unk_3c();
}
