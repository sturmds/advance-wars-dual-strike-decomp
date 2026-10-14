#include "types.h"
#include "game/force_rank_screen.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_027c0038[];

/* Queues the label of columns 1 to 10 on the main screen, 24 pixels apart
 * at y 40: one tile set for columns that list skills, another for empty ones. */
void ForceRankScreen_DrawColumnHeads(void) {
  int column;

  for (column = 1; column <= FORCE_RANK_COLUMN_SPECIAL; column++) {
    if (gForceRankScreen->columns[column].count != 0) {
      Sprite_QueueAt(1, ((column - 1) * 24 + 5) & 0x1ff, 40, data_027c0038,
                     (column * 2 + 44) | 0x2000, 0);
    } else {
      Sprite_QueueAt(1, ((column - 1) * 24 + 5) & 0x1ff, 40, data_027c0038,
                     (column * 2 + 66) | 0x2000, 0);
    }
  }
}
