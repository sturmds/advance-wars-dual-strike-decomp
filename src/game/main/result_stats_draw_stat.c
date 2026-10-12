#include "types.h"
#include "game/task.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
BOOL Sprite_IsOnScreenX(int x, int width);
void ResultStats_DrawCount(int x, int y, int value);

extern u8 data_027c0000;
extern u8 data_022a7ad8[][40];

/* Draws one stat of player task->unk_66 at (x, y) of the sub screen: a
 * label sprite, then its count (data_022a7ad8 holds 40 bytes per player). */
void ResultStats_DrawStat(int x, int y, int stat, Task* task) {
  if (!Sprite_IsOnScreenX(x, 40)) {
    return;
  }
  Sprite_QueueAt(0, x, y, &data_027c0000, (task->unk_66 + 10) << 12,
                 134 + stat * 8);
  ResultStats_DrawCount(x + 16, y + 16, data_022a7ad8[task->unk_66][stat]);
}
