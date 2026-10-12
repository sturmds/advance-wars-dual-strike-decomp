#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Gfx_RequestLayerCopy(int a);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Text_PrintImmediate(int x, int y, void* target, u32 text, int a, int b);
extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

void func_02089c9c(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Text_PrintImmediate(7, 11, data_027c0280->layer0Map, 0x4400000e, 0, 0);
  Text_PrintImmediate(7, 13, data_027c0280->layer0Map, 0x4400000f, 0, 0);
  Text_PrintImmediate(7, 15, data_027c0280->layer0Map, 0x44000010, 0, 0);
  Text_PrintImmediate(7, 17, data_027c0280->layer0Map, 0x44000011, 0, 0);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
