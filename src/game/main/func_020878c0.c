#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/screen_other_map.h"
#include "game/oam_cells.h"

extern Unk_02160ac0* data_02160ac0;
extern u8 data_02160b9c[];

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

/* Queues two sprites on the second screen, using data_02160ac0->unk_7e to pick their tiles. */
void func_020878c0(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Sprite_QueueAt(0, 64, 36, data_02160b9c, 0x3000,
                 (data_02160ac0->unk_7e << 8) + 0x400);
  Sprite_QueueAt(2, 112, 143, &data_027c0048, 0,
                 (data_02160ac0->unk_7e << 4) + 0x50);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
