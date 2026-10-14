#include "types.h"
#include "game/task.h"
#include "game/mode_font_task.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Task step: draws the line of mode font letters with an end piece on each
 * side. The end pieces sit 16 pixels higher, in the next sprite list and
 * the next palette slot, from tiles tile + 224 (left) and tile + 288
 * (right). */
void ModeFontTask_Draw(ModeFontTask* task) {
  int i;
  u16 x = task->x;
  u16 y = task->y;

  Sprite_QueueAt(task->unk_5c + 1, (x - 42) & 0x1ff, (y - 16) & 0xff,
                 &data_027c0030, task->unk_56 | ((task->paletteSlot + 1) << 12),
                 task->tile + 224);
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
  Sprite_QueueAt(task->unk_5c + 1, (x - 19) & 0x1ff, (y - 16) & 0xff,
                 &data_027c0030, task->unk_56 | ((task->paletteSlot + 1) << 12),
                 task->tile + 288);
}
