#include "types.h"
#include "game/task.h"
#include "game/mode_font_task.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_027c0000; /* cell table of one 16x32 sprite */

/* Task step: draws the line of mode font letters, without end pieces. */
void ModeFontTask_DrawPlain(ModeFontTask* task) {
  int i;
  u16 x = task->x;
  u16 y = task->y;

  for (i = 0; i < task->length; i++) {
    if (task->glyphs[i] == MODE_FONT_SPACE) {
      x += 14;
    } else {
      Sprite_QueueAt(task->unk_5c, x & 0x1ff, y & 0xff, &data_027c0000,
                     task->unk_56 | (task->paletteSlot << 12),
                     task->tile + task->glyphs[i] * 8);
      x += data_02159814[task->glyphs[i]] - 2;
    }
  }
}
