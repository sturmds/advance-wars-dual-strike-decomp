#include "types.h"
#include "game/tab_list.h"
#include "game/Unk_02088d04.h"

void Bg_PutTileQuad(int x, int y, int firstTile, int palette);
void func_020153d4(s16 x, s16 y, u16 *map, u32 textId, u16 a, u16 b);
void func_02014d14(s16 x, s16 y, u16 *map, int value, u16 a, u16 b);
extern Unk_027c0280 *data_027c0280;

void func_02071e90(int x, int y, int row, int col)
{
    u16 *map;

    Bg_PutTileQuad(x, y, 0x3e8, 2);
    func_020153d4(x + 2, y, data_027c0280->layer0Map, data_0215d6c4[row][col].textId, 0, ((y - 5) >> 1) * 64 + 128);
     
    map = data_027c0280->layer0Map;
    func_02014d14(x + 20, y, map, data_0215d6c4[row][col].getValue(row, col), 0, 0);
}
