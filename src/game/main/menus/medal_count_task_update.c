#include "types.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void Screen_SetCurrentGfx(void* state);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Gfx_RequestLayerCopy(int layer);
void TabList_CountMedals(u16* earned, u16* total);

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_027c0040;
extern u32 data_027c0098[];
extern char data_02167c88[]; /* "/" */

/* Task update that shows the medal count on the second screen
 * (data_027c0288.unk_04): two sprites, the medal heading and "earned /
 * total" on layer 1. */
void MedalCountTask_Update(Task* task) {
  u16 earned;
  u16 total;
  Unk_027c0280* saved;

  TabList_CountMedals(&earned, &total);
  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Sprite_QueueAt(4, 128, 144, &data_027c0098, 0x480e, 0);
  Sprite_QueueAt(3, 136, 152, &data_027c0040, 0x380a, 0);
  Text_PrintId(19, 19, data_027c0280->layer1Map, 0x7a000000, 0, 0x200);
  Number_Draw(26, 19, data_027c0280->layer1Map, earned, 0, 0);
  Text_Print(27, 19, data_027c0280->layer1Map, data_02167c88, 0, 0x1f0);
  Number_Draw(30, 19, data_027c0280->layer1Map, total, 0, 0);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentGfx(saved);
}
