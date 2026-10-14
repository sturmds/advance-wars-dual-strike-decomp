#include "types.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues a row of four sprites on the first screen, 40 pixels apart, with consecutive tiles. */
void func_0208798c(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  int i;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (i = 0; i < 4; i++) {
    Sprite_QueueAt(2, (52 + i * 40) & 0x1ff, 80, &data_027c0048, 0,
                   80 + i * 16);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
