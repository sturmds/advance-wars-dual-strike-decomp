#include "types.h"
#include "game/tab_list.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void Bg_PutTileQuad(int x, int y, int firstTile, int palette);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
extern Unk_027c0280* data_027c0280;

void func_02071cb0(int x, int y, int row, int col) {
  u16* map;

  Bg_PutTileQuad(x, y, 0x3f0, 2);
  Text_PrintId(x + 2, y, data_027c0280->layer0Map,
               data_0215d6c4[row][col].textId, 0, ((y - 5) >> 1) * 64 + 128);

  map = data_027c0280->layer0Map;
  Number_Draw(x + 20, y, map, data_0215d6c4[row][col].getValue(row, col), 0, 0);
}
